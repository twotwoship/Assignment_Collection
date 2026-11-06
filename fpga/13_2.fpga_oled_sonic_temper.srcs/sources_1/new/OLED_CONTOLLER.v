`timescale 1ns / 1ps
// CTRL[2] = START_EN    CTRL[1] = STOP_EN    CTRL[0] = RW
// STATUS[2:0] = {busy, done, nack_error}
//
// SSD1306 / 128 x 32
// OLED에 연결된 I2C_MASTER의 slave_addr = 7'h3C
//
// value[47:40] = 습도 정수
// value[39:32] = 습도 소수 한 자리
// value[31:24] = 온도 정수
// value[23:16] = 온도 소수 한 자리
// value[15:8]  = 거리 상위 바이트
// value[7:0]   = 거리 하위 바이트
// 예: value = 48'h170522050022
// T&A:34.5C/23.5%
// dist:34cm

module OLED_CONTOLLER(
    input clk, reset, [47:0]value, [2:0]status, [7:0]rx_data,
    output reg request, output reg [2:0]ctrl, output reg [7:0]tx_data
);

parameter power_wait   = 5'b00000;
parameter init         = 5'b00001;
parameter clear_screen = 5'b00010;
parameter wait_update  = 5'b00011;
parameter latch_value  = 5'b00100;
parameter set_position = 5'b00101;
parameter draw         = 5'b00110;

parameter prep_byte    = 5'b00111;
parameter wait_ready   = 5'b01000;
parameter issue        = 5'b01001;
parameter wait_done    = 5'b01010;
parameter byte_return  = 5'b01011;
parameter return_state = 5'b01100;
parameter byte_error   = 5'b01101;
parameter error        = 5'b01110;

// 32x32 그림: 위에서 아래로, 각 행의 MSB가 왼쪽 픽셀 삐츄
localparam [1023:0] PICHU_BITMAP = {
    32'h00000000, 32'h40000000, 32'h70000000, 32'h78000000,
    32'h7C000000, 32'h7E000000, 32'h7F000000, 32'h7FC0001E,
    32'h77E000FE, 32'h71F007FE, 32'h70701FFC, 32'h70303E3C,
    32'h70207838, 32'h78206078, 32'h38404070, 32'h387F40F0,
    32'h1C40C0E0, 32'h060001C0, 32'h040007C0, 32'h08001F80,
    32'h09800800, 32'h13404400, 32'h13C3C400, 32'h11808400,
    32'h14102400, 32'h0E007800, 32'h0E006800, 32'h04003000,
    32'h03006000, 32'h00FF8000, 32'h00000000, 32'h00000000
};

// 100MHz 기준
parameter POWER_WAIT_COUNT = 10_000_000; // 전원 안정화 100ms
parameter UPDATE_COUNT     = 10_000_000; // 화면 전송 완료 후 100ms 대기

reg [4:0] r_state;
reg [4:0] r_job;
reg [4:0] r_return_state;

reg [31:0] r_count;
reg [9:0] r_index;
reg [9:0] r_last_index;
reg [1:0] r_phase;

// 한 화면을 출력하는 동안 사용할 센서 값
reg [47:0] r_value;

// 한 줄당 ASCII 문자 21개
reg [167:0] r_line1;
reg [167:0] r_line2;

// 이진수 → 십진수 자릿값 변환용
reg [4:0] r_convert_count;
reg [15:0] r_binary_temp;
reg [15:0] r_binary_hum;
reg [15:0] r_binary_dist;

reg [19:0] r_bcd_temp;
reg [19:0] r_bcd_hum;
reg [19:0] r_bcd_dist;

// 이번에 전송할 한 바이트
reg [7:0] r_byte;
reg [2:0] r_byte_ctrl;


//----------------------------------------------------------
// OLED 초기화 명령
//
// index 0 : 명령용 제어 바이트 0x00
// index 1~31 : 명령과 명령 인자
//----------------------------------------------------------
function [7:0] init_byte;
    input [9:0] index;
    begin
        case(index)
            0:  init_byte = 8'h00;
            1:  init_byte = 8'hAE; // 화면 OFF
            2:  init_byte = 8'hD5; // 내부 표시 클럭 설정
            3:  init_byte = 8'h80;
            4:  init_byte = 8'hA8; // MUX 설정: 32줄
            5:  init_byte = 8'h1F;
            6:  init_byte = 8'hD3; // 표시 오프셋
            7:  init_byte = 8'h00;
            8:  init_byte = 8'h40; // 표시 시작 줄 0
            9:  init_byte = 8'h8D; // 내부 차지 펌프 ON
            10: init_byte = 8'h14;
            11: init_byte = 8'h20; // 수평 주소 지정 방식
            12: init_byte = 8'h00;
            13: init_byte = 8'hA1; // SEG 방향
            14: init_byte = 8'hC8; // COM 방향
            15: init_byte = 8'hDA; // 128x32 COM 설정
            16: init_byte = 8'h02;
            17: init_byte = 8'h81; // 명암 설정
            18: init_byte = 8'h8F;
            19: init_byte = 8'hD9; // 프리차지 설정
            20: init_byte = 8'hF1;
            21: init_byte = 8'hDB; // VCOMH 설정
            22: init_byte = 8'h40;
            23: init_byte = 8'hA4; // 화면 RAM 내용 표시
            24: init_byte = 8'hA6; // 정상 표시
            25: init_byte = 8'h2E; // 스크롤 OFF
            26: init_byte = 8'h21; // 열 범위
            27: init_byte = 8'h00; // 시작 열 0
            28: init_byte = 8'h7F; // 마지막 열 127
            29: init_byte = 8'h22; // 페이지 범위
            30: init_byte = 8'h00; // 시작 페이지 0
            31: init_byte = 8'h03; // 마지막 페이지 3
            default: init_byte = 8'h00;
        endcase
    end
endfunction

//----------------------------------------------------------
// 화면 갱신 전 쓰기 범위와 시작 위치 설정
// 화면을 지운 뒤 이 과정에서 화면을 켬
//----------------------------------------------------------
function [7:0] position_byte;
    input [9:0] index;
    begin
        case(index)
            0: position_byte = 8'h00; // 명령용 제어 바이트
            1: position_byte = 8'h21;
            2: position_byte = 8'h00;
            3: position_byte = 8'h7F;
            4: position_byte = 8'h22;
            5: position_byte = 8'h00;
            6: position_byte = 8'h03;

            7: position_byte = 8'hAF; // 화면 ON

            default: position_byte = 8'h00;
        endcase
    end
endfunction

//----------------------------------------------------------
// 이진수를 십진수 자릿값으로 변환하는 한 단계
//
// 각 4비트 자릿값이 5 이상이면 3을 더하고,
// 왼쪽으로 이동하면서 이진수 한 비트를 넣음.
//
// 16비트 입력에 대해 16 CLK 반복
//----------------------------------------------------------
function [19:0] bcd_step;
    input [19:0] bcd;
    input binary_bit;

    integer digit;
    reg [19:0] adjusted;

    begin
        adjusted = bcd;

        for(digit = 0; digit < 5; digit = digit + 1) begin
            if(adjusted[digit*4 +: 4] >= 5) begin
                adjusted[digit*4 +: 4] =
                    adjusted[digit*4 +: 4] + 4'd3;
            end
        end

        bcd_step = {adjusted[18:0], binary_bit};
    end
endfunction

//----------------------------------------------------------
// 첫째 줄 문자열 생성
// T&A:34.5C/23.5%
// t, h는 변환된 십진수 자릿값
// 예: 십진수 34 → t = 12'h034
//----------------------------------------------------------
function [167:0] format_line1;
    input [47:0] data;
    input [11:0] t;
    input [11:0] h;
    integer p;
    reg [167:0] text;
    begin
        text = {21{8'h20}}; // 전체 공백
        text[167 -: 32] = "T&A:";
        p = 4;

        // 온도 정수: 앞의 불필요한 0은 생략
        if(t[11:8] != 0) begin
            text[167-p*8 -: 8] = 8'd48 + t[11:8];
            p = p + 1;
        end

        if(t[11:4] != 0) begin
            text[167-p*8 -: 8] = 8'd48 + t[7:4];
            p = p + 1;
        end

        text[167-p*8 -: 8] = 8'd48 + t[3:0];
        p = p + 1;

        text[167-p*8 -: 8] = ".";
        p = p + 1;

        // 온도 소수 한 자리
        if(data[23:16] <= 9)
            text[167-p*8 -: 8] = 8'd48 + data[23:16];
        else
            text[167-p*8 -: 8] = "?";

        p = p + 1;

        text[167-p*8 -: 8] = "C";
        p = p + 1;

        text[167-p*8 -: 8] = "/";
        p = p + 1;

        // 습도 정수
        if(h[11:8] != 0) begin
            text[167-p*8 -: 8] = 8'd48 + h[11:8];
            p = p + 1;
        end

        if(h[11:4] != 0) begin
            text[167-p*8 -: 8] = 8'd48 + h[7:4];
            p = p + 1;
        end

        text[167-p*8 -: 8] = 8'd48 + h[3:0];
        p = p + 1;

        text[167-p*8 -: 8] = ".";
        p = p + 1;

        // 습도 소수 한 자리
        if(data[39:32] <= 9)
            text[167-p*8 -: 8] = 8'd48 + data[39:32];
        else
            text[167-p*8 -: 8] = "?";

        p = p + 1;

        text[167-p*8 -: 8] = "%";

        format_line1 = text;
    end
endfunction

//----------------------------------------------------------
// 둘째 줄 문자열 생성
// dist:34cm
//----------------------------------------------------------
function [167:0] format_line2;
    input [19:0] d;

    integer p;
    reg [167:0] text;

    begin
        text = {21{8'h20}};
        text[167 -: 40] = "dist:";
        p = 5;

        if(d[19:16] != 0) begin
            text[167-p*8 -: 8] = 8'd48 + d[19:16];
            p = p + 1;
        end

        if(d[19:12] != 0) begin
            text[167-p*8 -: 8] = 8'd48 + d[15:12];
            p = p + 1;
        end

        if(d[19:8] != 0) begin
            text[167-p*8 -: 8] = 8'd48 + d[11:8];
            p = p + 1;
        end

        if(d[19:4] != 0) begin
            text[167-p*8 -: 8] = 8'd48 + d[7:4];
            p = p + 1;
        end

        text[167-p*8 -: 8] = 8'd48 + d[3:0];
        p = p + 1;

        text[167-p*8 -: 8] = "c";
        p = p + 1;

        text[167-p*8 -: 8] = "m";

        format_line2 = text;
    end
endfunction

function [39:0] font;
    input [7:0] ch;
    begin
        case(ch)
            "0": font = {8'h3E,8'h45,8'h49,8'h51,8'h3E};
            "1": font = {8'h00,8'h40,8'h7F,8'h42,8'h00};
            "2": font = {8'h46,8'h49,8'h51,8'h61,8'h42};
            "3": font = {8'h31,8'h4B,8'h45,8'h41,8'h21};
            "4": font = {8'h10,8'h7F,8'h12,8'h14,8'h18};
            "5": font = {8'h39,8'h45,8'h45,8'h45,8'h27};
            "6": font = {8'h30,8'h49,8'h49,8'h4A,8'h3C};
            "7": font = {8'h03,8'h05,8'h09,8'h71,8'h01};
            "8": font = {8'h36,8'h49,8'h49,8'h49,8'h36};
            "9": font = {8'h1E,8'h29,8'h49,8'h49,8'h06};
            "T": font = {8'h01,8'h01,8'h7F,8'h01,8'h01};
            "&": font = {8'h50,8'h22,8'h55,8'h49,8'h36};
            "A": font = {8'h7E,8'h11,8'h11,8'h11,8'h7E};
            "C": font = {8'h22,8'h41,8'h41,8'h41,8'h3E};
            "d": font = {8'h7F,8'h48,8'h48,8'h48,8'h30};
            "i": font = {8'h00,8'h40,8'h7D,8'h44,8'h00};
            "s": font = {8'h20,8'h54,8'h54,8'h54,8'h48};
            "t": font = {8'h20,8'h40,8'h44,8'h3F,8'h04};
            "c": font = {8'h20,8'h44,8'h44,8'h44,8'h38};
            "m": font = {8'h78,8'h04,8'h18,8'h04,8'h7C};
            ":": font = {8'h00,8'h00,8'h36,8'h00,8'h00};
            ".": font = {8'h00,8'h60,8'h60,8'h00,8'h00};
            "/": font = {8'h02,8'h04,8'h08,8'h10,8'h20};
            "%": font = {8'h62,8'h64,8'h08,8'h13,8'h23};
            "?": font = {8'h06,8'h09,8'h51,8'h01,8'h02};
            default: font = 40'h0000000000;
        endcase
    end
endfunction

//----------------------------------------------------------
// 화면 주소에 해당하는 픽셀 한 바이트 생성
//
// 글자 한 칸: 가로 6픽셀, 세로 16픽셀
// 5x7 글꼴을 세로로 2배 확대하고 나머지는 공백
//
// page 0, 1 : 첫째 줄
// page 2, 3 : 둘째 줄
//----------------------------------------------------------
function [7:0] pixel_byte;
    input [8:0] address;
    input [167:0] line1;
    input [167:0] line2;

    integer x;
    integer page;
    integer character;
    integer column;
    integer bit_number;
    integer i;

    reg [7:0] ch;
    reg [7:0] dots;
    reg [39:0] glyph;
    reg [15:0] tall;

    begin
        x = address[6:0];
        page = address[8:7];

        character = x / 6;
        column = x % 6;

        ch = 8'h20;

        if(character < 21) begin
            if(page < 2) ch = line1[167-character*8 -: 8];
            else ch = line2[167-character*8 -: 8];
        end
        glyph = font(ch); dots = 8'h00;

        if(column < 5) dots = glyph[column*8 +: 8];
        tall = 16'h0000;
        for(bit_number = 0; bit_number < 7; bit_number = bit_number + 1) begin
            tall[bit_number*2]   = dots[bit_number];
            tall[bit_number*2+1] = dots[bit_number];
        end

        if(page % 2 == 0) pixel_byte = tall[7:0];
        else pixel_byte = tall[15:8];
        
        if(x >= 96) begin
            for(i = 0; i < 8; i = i + 1) begin
                pixel_byte[i] = PICHU_BITMAP[ 1023 - ((page * 8 + i) * 32 + (x - 96)) ];
            end
        end
    end
endfunction

//----------------------------------------------------------
// OLED 제어 FSM
//----------------------------------------------------------
always @(posedge clk or posedge reset) begin
    if(reset) begin
        r_state <= power_wait;
        r_job <= init;
        r_return_state <= power_wait;

        r_count <= 0;
        r_index <= 0;
        r_last_index <= 0;
        r_phase <= 0;

        r_value <= 48'h000000000000;
        r_line1 <= {21{8'h20}};
        r_line2 <= {21{8'h20}};

        r_convert_count <= 0;

        r_binary_temp <= 0;
        r_binary_hum <= 0;
        r_binary_dist <= 0;

        r_bcd_temp <= 0;
        r_bcd_hum <= 0;
        r_bcd_dist <= 0;

        r_byte <= 8'h00;
        r_byte_ctrl <= 3'b000;

        request <= 1'b0;
        ctrl <= 3'b000;
        tx_data <= 8'h00;

    end else begin
        // ISSUE 상태에서만 한 CLK 동안 1
        request <= 1'b0;

        case(r_state)

        //--------------------------------------------------
        // 전원 안정화 대기
        //--------------------------------------------------
        power_wait: begin
            if(r_count >= POWER_WAIT_COUNT - 1) begin
                r_count <= 0;
                r_state <= init;
            end else begin
                r_count <= r_count + 1'b1;
            end
        end

        //--------------------------------------------------
        // 초기화 명령 전송 준비
        //--------------------------------------------------
        init: begin
            r_job <= init;
            r_index <= 0;
            r_last_index <= 31;

            r_return_state <= clear_screen;
            r_phase <= 0;
            r_state <= prep_byte;
        end

        //--------------------------------------------------
        // 0x40 + 화면 데이터 512바이트 전송 준비
        //--------------------------------------------------
        clear_screen: begin
            r_job <= clear_screen;
            r_index <= 0;
            r_last_index <= 512;

            r_return_state <= wait_update;
            r_phase <= 0;
            r_state <= prep_byte;
        end

        //--------------------------------------------------
        // 화면 갱신 주기 대기
        //--------------------------------------------------
        wait_update: begin
            if(r_count >= UPDATE_COUNT - 1) begin
                r_count <= 0;
                r_phase <= 0;
                r_state <= latch_value;
            end else begin
                r_count <= r_count + 1'b1;
            end
        end

        //--------------------------------------------------
        // 센서 값 저장 → 숫자 변환 → 문자열 생성
        //--------------------------------------------------
        latch_value: begin
            case(r_phase)
                0: begin
                    // 이 값으로 한 화면 전체를 출력
                    r_value <= value; r_phase <= 1;
                end
                1: begin
                    // 정수 부분을 16비트 이진수로 준비
                    r_binary_temp <= {8'h00, r_value[31:24]};
                    r_binary_hum <= {8'h00, r_value[47:40]};
                    r_binary_dist <= r_value[15:0];

                    r_bcd_temp <= 0;
                    r_bcd_hum <= 0;
                    r_bcd_dist <= 0;

                    r_convert_count <= 0;
                    r_phase <= 2;
                end
                2: begin
                    // 한 CLK에 한 비트씩 십진수로 변환
                    r_bcd_temp <= bcd_step(r_bcd_temp, r_binary_temp[15]);
                    r_bcd_hum <= bcd_step(r_bcd_hum, r_binary_hum[15]);
                    r_bcd_dist <= bcd_step(r_bcd_dist, r_binary_dist[15]);
                    r_binary_temp <= {r_binary_temp[14:0], 1'b0};
                    r_binary_hum <= {r_binary_hum[14:0], 1'b0};
                    r_binary_dist <= {r_binary_dist[14:0], 1'b0};
                    if(r_convert_count == 15) begin
                        r_phase <= 3;
                    end else begin
                        r_convert_count <=r_convert_count + 1'b1;
                    end
                end
                3: begin
                    r_line1 <= format_line1( r_value, r_bcd_temp[11:0], r_bcd_hum[11:0]);
                    r_line2 <= format_line2(r_bcd_dist);
                    r_phase <= 0;
                    r_state <= set_position;
                end
            endcase
        end
        set_position: begin
            r_job <= set_position;
            r_index <= 0;
            r_last_index <= 7;

            r_return_state <= draw;
            r_phase <= 0;
            r_state <= prep_byte;
        end
        draw: begin
            r_job <= draw;
            r_index <= 0;
            r_last_index <= 512;

            r_return_state <= wait_update;
            r_phase <= 0;
            r_state <= prep_byte;
        end
        prep_byte: begin
            case(r_phase)
                0: begin
                    case(r_job)
                        init: begin
                            r_byte <= init_byte(r_index);
                        end

                        clear_screen: begin
                            if(r_index == 0)
                                r_byte <= 8'h40;
                            else
                                r_byte <= 8'h00;
                        end

                        set_position: begin
                            r_byte <= position_byte(r_index);
                        end

                        draw: begin
                            if(r_index == 0) begin
                                r_byte <= 8'h40;
                            end else begin
                                r_byte <= pixel_byte(
                                    r_index - 10'd1,
                                    r_line1,
                                    r_line2
                                );
                            end
                        end

                        default: begin
                            r_byte <= 8'h00;
                        end
                    endcase

                    // 첫 바이트: START + 주소 + 제어 바이트
                    if(r_index == 0)
                        r_byte_ctrl <= 3'b100;

                    // 마지막 바이트: 쓰기 후 STOP
                    else if(r_index == r_last_index)
                        r_byte_ctrl <= 3'b010;

                    // 중간 바이트: STOP 없이 쓰기
                    else
                        r_byte_ctrl <= 3'b000;

                    r_phase <= 1;
                end

                1: begin
                    // 앞 CLK에서 준비한 값을 출력에 저장
                    tx_data <= r_byte;
                    ctrl <= r_byte_ctrl;

                    r_phase <= 0;
                    r_state <= wait_ready;
                end
            endcase
        end
        wait_ready: begin
            if(!status[2]) begin
                r_state <= issue;
            end
        end
        issue: begin
            request <= 1'b1;
            r_state <= wait_done;
        end
        wait_done: begin
            if(status[1]) begin
                if(status[0]) begin
                    r_state <= byte_error;
                end else begin
                    r_state <= byte_return;
                end
            end
        end
        byte_return: begin
            if(r_index == r_last_index) begin
                r_state <= return_state;
            end else begin
                r_index <= r_index + 1'b1;
                r_phase <= 0;
                r_state <= prep_byte;
            end
        end
        return_state: begin
            r_count <= 0;
            r_index <= 0;
            r_phase <= 0;

            r_state <= r_return_state;
        end
        byte_error: begin
            // 첨부한 I2C_MASTER는 NACK 시 STOP을 끝낸 뒤
            // nack_error와 done을 보고함
            r_state <= error;
        end
        error: begin
            r_count <= 0;
            r_index <= 0;
            r_phase <= 0;

            r_state <= power_wait;
        end

        default: begin
            r_count <= 0;
            r_index <= 0;
            r_phase <= 0;

            r_state <= power_wait;
        end

        endcase
    end
end
// OLED에는 쓰기만 하므로 rx_data는 내부에서 사용하지 않음.
endmodule
