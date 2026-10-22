`timescale 1ns / 1ps
module fnd_controllor(input clk, reset, tick, mode_sel, mode2_sel, [23:0]in_data, output [3:0]an, [7:0]seg);
    parameter NS_LIMIT = 100_000_0;
    wire [1:0] w_sel;
    fnd_digit_select u_fnd_digit_select(.clk(clk), .reset(reset), .tick(tick), .sel(w_sel));
    wire [3:0]w_d1, w_d10, w_d100, w_d1000;
    wire [3:0]w_sw_d1, w_sw_d10, w_sw_d100, w_sw_d1000;
    wire [3:0]w_c_d1, w_c_d10, w_c_d100, w_c_d1000;

    wire [3:0] w_di_an; wire [7:0] w_di_seg;
    wire [3:0] w_ani_an; wire [7:0] w_ani_seg;

    //자 모드 투 만들었어 메인에서 스탑일때 5초 지나면 모드투를 1로 만들어가지고 신호를 보내주면 여기 최종 아웃풋 에서 먹스 선택해서 뽑게 하는거임.
    // 기존라인 신경전혀안쓰게 // mode2_sel 에서 0이면 스탑와치 1이면  스탑와치 스톱 5초지남
    //mode_sel 이 0일때  u_clock_bin2bcd4_digit 실행 0일 때 시계인거고
    //mode_sel 이 1일때 u_bin2bcd4_digit 실행 1일 때 스톱워치인거임
    // 1초마다 on off되게 d100의 점을 껏다 ㅕㅋㅆ다햐애됨
    bin2bcd4_digit u_bin2bcd4_digit(.in_data(in_data), .d1(w_sw_d1), .d10(w_sw_d10), .d100(w_sw_d100), .d1000(w_sw_d1000));
    clock_bin2bcd4_digit u_clock_bin2bcd4_digit(.in_data(in_data), .d1(w_c_d1), .d10(w_c_d10), .d100(w_c_d100), .d1000(w_c_d1000));
    assign w_d1 = mode_sel ? w_sw_d1 : w_c_d1;
    assign w_d10 = mode_sel ? w_sw_d10 : w_c_d10;
    assign w_d100 = mode_sel ? w_sw_d100 : w_c_d100;
    assign w_d1000 = mode_sel ? w_sw_d1000 : w_c_d1000;
    fnd_digit_display #(.NS_LIMIT(NS_LIMIT))u_fnd_digit_display(.clk(clk), .reset(reset), .mode_sel(mode_sel),.mode2_sel(mode2_sel), .sel(w_sel), 
                                            .d1(w_d1), .d10(w_d10), .d100(w_d100), .d1000(w_d1000), .an(w_di_an), .seg(w_di_seg));
    fnd_ani_display #(.NS_LIMIT(NS_LIMIT))u_fnd_ani_display(.clk(clk), .reset(reset), .an(w_ani_an), .seg(w_ani_seg));
    assign an = mode2_sel ? w_ani_an : w_di_an;
    assign seg = mode2_sel ? w_ani_seg : w_di_seg;
endmodule

// 빙빙 돌아가는
module fnd_ani_display(input clk, reset, output reg [3:0]an, reg [7:0]seg);
    reg [26:0]r_10ns_counter = 0; reg [3:0]r_1ms_counter = 0;
    parameter NS_LIMIT = 100_000_0;
    always@(posedge clk or posedge reset) begin
        if(reset) begin
            r_10ns_counter <= 0; r_1ms_counter <= 0;
        end
        else begin
            if(r_10ns_counter == NS_LIMIT - 1) begin
                r_10ns_counter <= 0; r_1ms_counter <= r_1ms_counter + 1;
                if(r_1ms_counter == 12) r_1ms_counter <= 0; 
            end else begin
                r_10ns_counter <= r_10ns_counter + 1;
            end
        end
    end
    // 타임 an 하고 seg는 구분되어야한다 r_1ms_counter가 바뀔때마다 an은 한칸씩 옆으로 이동되어야 하며 아닌가? 안키면 되는거아닐까?
    // 모두가 다 출력이 들어가지만 특정상황일때 특정만 켜지는거지 아닌가? 돌아가게 만들어야함.
    // 일단 제일 위 껍데기는 이게 맞다 1ms 마다 바뀌어야하니까 1ms가 바뀔때마다 값이 정해져야됨
    always@(r_1ms_counter) begin
        case(r_1ms_counter)
            4'd0: begin seg = 8'b11111110; an = 4'b1110; end 
            4'd1: begin seg = 8'b11111110; an = 4'b1101; end 
            4'd2: begin seg = 8'b11111110; an = 4'b1011; end 
            4'd3: begin seg = 8'b11111110; an = 4'b0111; end 
            4'd4: begin seg = 8'b11011111; an = 4'b0111; end 
            4'd5: begin seg = 8'b11101111; an = 4'b0111; end 
            4'd6: begin seg = 8'b11110111; an = 4'b0111; end 
            4'd7: begin seg = 8'b11110111; an = 4'b1011; end 
            4'd8: begin seg = 8'b11110111; an = 4'b1101; end 
            4'd9: begin seg = 8'b11110111; an = 4'b1110; end 
            4'd10: begin seg = 8'b11111011; an = 4'b1110; end 
            4'd11: begin seg = 8'b11111101; an = 4'b1110; end 
            default: begin seg = 8'b11111111; an = 4'b1111; end  // 아니면 꺼져있기
        endcase
    end
endmodule

// logic select 1_digit displaying fnd every 1ms, 
module fnd_digit_select(input clk, reset, tick, output reg[1:0] sel);
    always @(posedge reset, posedge tick) begin
        if(reset) sel <= 0; else begin if(tick) sel <= sel + 1; end
    end
endmodule

// in_data : bin size express to 9999 in 14bit fnd // 0000 ~ 9999 /digit number 0 ~ 9 bcd 4bit express
module bin2bcd4_digit(input [23:0] in_data, output [3:0]d1, [3:0]d10, [3:0]d100, [3:0]d1000);
    assign d1 = in_data % 10;               assign d10 = (in_data / 10) % 10;
    assign d100 = (in_data / 100) % 10;     assign d1000 = (in_data / 1000) % 10;
endmodule

// 어케 쪼개야될까? 쪼개? 국왕님~
module clock_bin2bcd4_digit(input [23:0] in_data, output [3:0]d1, [3:0]d10, [3:0]d100, [3:0]d1000);
// 들어오는 값음 미리초임 초단위로 계산해야됨 들어오는게 10ms 단위임 100들어오면 1s임 일단 기본적으로 100으로 쪼개고 시작해 
    assign d1 = ((in_data / 100) % 60) % 10;               assign d10 = ((in_data / 100) % 60) / 10;
    assign d100 = ((in_data / 100) / 60) % 10;     assign d1000 = ((in_data / 100) / 60) / 10;
endmodule

module fnd_digit_display(input clk, reset, mode_sel, mode2_sel, input [1:0]sel, [3:0]d1, [3:0]d10, [3:0]d100, [3:0]d1000, output reg [3:0]an, reg [7:0]seg);
    parameter NS_LIMIT = 100_000_0;
    reg [3:0] bcd_data;
    reg [26:0]r_10ns_counter; reg r_1s_t = 0;
    always@(posedge clk or posedge reset) begin
        if(reset) begin r_10ns_counter <= 0; r_1s_t <= 0; end
        else begin
            if(r_10ns_counter == (NS_LIMIT * 100) - 1) begin
                r_10ns_counter <= 0; r_1s_t <= ~r_1s_t;
            end else r_10ns_counter <= r_10ns_counter + 1;
        end
    end
    // 지금 구조가 넘어온 d값에 따라 아웃 seg가 결정됨  b
    always@(sel) begin
        case(sel)
            2'b00: begin bcd_data = d1;     an = 4'b1110; end
            2'b01: begin bcd_data = d10;    an = 4'b1101; end
            2'b10: begin bcd_data = d100;   an = 4'b1011; end
            2'b11: begin bcd_data = d1000;  an = 4'b0111; end
            default: begin bcd_data = 4'b0000; an = 4'b1111; end
        endcase
    end
    always @(*) begin
    case(bcd_data)
        4'd0: seg = 8'b11000000;
        4'd1: seg = 8'b11111001;
        4'd2: seg = 8'b10100100;
        4'd3: seg = 8'b10110000;
        4'd4: seg = 8'b10011001;
        4'd5: seg = 8'b10010010;
        4'd6: seg = 8'b10000010;
        4'd7: seg = 8'b11111000;
        4'd8: seg = 8'b10000000;
        4'd9: seg = 8'b10010000;
        default: seg = 8'b11111111;
    endcase

    if(sel == 2'b10 && r_1s_t && ~mode_sel)
        seg[7] = 1'b0;     // 시계모드일때는 1초마다 번갈아 점 켜
    if(sel == 2'b10 && mode_sel && ~mode2_sel)
        seg[7] = 1'b0;     //스탑와치 돌아갈때는 계속 점 켜놔
end
endmodule

