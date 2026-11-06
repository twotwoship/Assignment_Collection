`timescale 1ns / 1ps
// CTRL[2] = START_EN    CTRL[1] = STOP_EN    CTRL[0] = RW
// STATUS[2:0] = {busy, done, nack_error}
module I2C_MASTER#(parameter [6:0]slave_addr = 7'h00)(input clk, reset, inout sda, scl, input request, 
[2:0]ctrl, [7:0]tx_data, output reg [7:0]rx_data, output reg [2:0]status);


parameter wait_req = 5'b00000; parameter start_ = 5'b00001;
parameter addr_bits = 5'b00010; parameter addr_ack = 5'b00011; 
parameter addr_error = 5'b00100; parameter route_rw = 5'b00101; 
parameter byte_tx = 5'b00110; parameter write_bits = 5'b00111; 
parameter read_bits = 5'b01000; parameter write_ack = 5'b01001; 
parameter force_stop = 5'b01010; parameter stop_ = 5'b01011; 
parameter post_byte = 5'b01100; parameter complete = 5'b01101; 
parameter read_resp = 5'b01110; parameter latch = 5'b01111;


// e도로도로동도오동기화
(* ASYNC_REG = "TRUE" *) reg [1:0] r_sda_sync;
wire sda_in; assign sda_in = r_sda_sync[1];
always @(posedge clk or posedge reset) begin
    if(reset) r_sda_sync <= 2'b11;
    else r_sda_sync <= {r_sda_sync[0], sda};
end

localparam QUARTER_COUNT = 250; // 100키로헤르츠
reg [7:0] r_div_count; wire tick_i2c;
assign tick_i2c = (r_div_count == QUARTER_COUNT - 1);
always @(posedge clk or posedge reset) begin
    if (reset) begin r_div_count <= 0; end 
    else if (tick_i2c) begin r_div_count <= 0; end 
    else begin r_div_count <= r_div_count + 1'b1; end
end

reg[4:0] r_state; reg[2:0] r_ctrl; reg[7:0] r_tx; reg r_sda_low; reg r_scl_low;
// 1이면 로우로 땡기고 0이면 놔주는걸 기억(외부 풀업으로 1이 됨 노예가 통제할수잇게)해라 반대임
assign sda = r_sda_low ? 1'b0 : 1'bz; assign scl = r_scl_low ? 1'b0 : 1'bz;

reg [2:0] r_phase; reg [2:0] r_bit_index;

wire [7:0]addr_byte;
assign addr_byte = {slave_addr, r_ctrl[0]};

always @(posedge clk, posedge reset) begin
    if(reset) begin
        r_state <= wait_req; r_tx <= 8'h00; status <= 3'b000;
        r_ctrl <= 3'b000; rx_data <= 8'h00; 
        r_sda_low <= 1'b0; r_scl_low <= 1'b0; r_phase <= 3'd0;
        r_bit_index <= 3'd7;
    end else begin
        status[1] <= 1'b0;
        case(r_state)
        wait_req: begin
            status[2] <= 1'b0;
            if(request) begin
                r_ctrl <= ctrl; r_tx<= tx_data;
                r_phase <= 3'd0; r_bit_index <= 3'd7;
                status[2] <= 1'b1; status[0] <= 1'b0;
                r_state <= latch;
            end
        end
        start_: begin
            if (tick_i2c) begin
                case (r_phase)
                    0: begin r_sda_low <= 1'b0; r_scl_low <= 1'b0; r_phase <= 1; end
                    1: begin r_phase <= 2; end
                    2: begin r_sda_low <= 1'b1; r_phase <= 3; end
                    3: begin r_phase <= 4; end
                    4: begin
                        r_scl_low <= 1'b1; r_bit_index <= 3'd7; 
                        r_phase <= 3'd1; r_state <= addr_bits; 
                    end
                endcase
            end
        end
        addr_bits: begin
            if (tick_i2c) begin
                case (r_phase)
                    0: begin
                        r_scl_low <= 1'b1;
                        r_phase   <= 1;
                    end
                    1: begin
                        r_sda_low <= ~addr_byte[r_bit_index];
                        r_phase   <= 2;
                    end
                    2: begin
                        r_scl_low <= 1'b0;
                        r_phase   <= 3;
                    end
                    3: begin
                        r_phase <= 0;
                        if (r_bit_index == 0) begin
                            r_state <= addr_ack;
                        end else begin
                            r_bit_index <= r_bit_index - 1'b1;
                        end
                    end
                endcase
            end
        end
        addr_ack: begin
            if(tick_i2c) begin
                case(r_phase)
                    0: begin
                        r_scl_low <= 1'b1;
                        r_phase <= 1;
                    end
                    1: begin
                        r_sda_low <= 1'b0;
                        r_phase <= 2;
                    end
                    2: begin
                        r_scl_low <= 1'b0;
                        r_phase <= 3;
                    end
                    3: begin
                        if(sda_in == 1'b0) begin
                            r_bit_index <= 3'd7; 
                            r_state <= route_rw;
                            r_phase <= 0;
                        end else begin
                            r_state <= addr_error;
                            r_phase <= 0;
                        end
                    end
                endcase
            end
        end
        addr_error: begin
            status[0] <= 1'b1;  // 주소 NACK 오류 기록
            r_phase <= 0;
            r_state <= stop_;
        end
        route_rw: begin
            r_state <= byte_tx;
        end
        byte_tx: begin
            if(r_ctrl[0]) r_state <= read_bits;
            else r_state <= write_bits;
        end
        write_bits: begin
            if(tick_i2c) begin
                case(r_phase)
                    0: begin
                        r_scl_low <= 1'b1;
                        r_phase   <= 1;
                    end
                    1: begin
                        r_sda_low <= ~r_tx[r_bit_index];
                        r_phase <= 2;
                    end
                    2: begin
                        r_scl_low <= 1'b0;
                        r_phase <= 3;
                    end
                    3: begin
                        if(r_bit_index == 0)begin
                            r_state <= write_ack;
                            r_phase <= 0;
                        end else begin
                            r_bit_index <= r_bit_index - 1'b1;
                            r_phase <= 0;
                        end
                    end
                endcase
            end
        end
        read_bits: begin
            if(tick_i2c) begin
                case(r_phase)
                    0: begin
                        r_scl_low <= 1'b1;
                        r_phase <= 1;
                    end
                    1: begin
                        // 슬레이브가 SDA를 구동하도록 해제
                        r_sda_low <= 1'b0;
                        r_phase <= 2;
                    end
                    2: begin
                        r_scl_low <= 1'b0;
                        r_phase <= 3;
                    end
                    3: begin
                        // SCL HIGH 구간에서 수신 비트 저장
                        rx_data[r_bit_index] <= sda_in;
                        r_phase <= 0;

                        if(r_bit_index == 0) begin
                            r_state <= read_resp;
                        end else begin
                            r_bit_index <= r_bit_index - 1'b1;
                        end
                    end
                endcase
            end
        end
        write_ack: begin
            if(tick_i2c) begin
                case(r_phase)
                    0: begin
                        r_scl_low <= 1'b1;
                        r_phase <= 1;
                    end
                    1: begin
                        // 응답은 슬레이브가 보내므로 SDA 해제
                        r_sda_low <= 1'b0;
                        r_phase <= 2;
                    end
                    2: begin
                        r_scl_low <= 1'b0;
                        r_phase <= 3;
                    end
                    3: begin
                        r_phase <= 0;

                        if(sda_in == 1'b0) begin
                            r_state <= post_byte;
                        end else begin
                            r_state <= force_stop;
                        end
                    end
                endcase
            end
        end
        force_stop: begin
            status[0] <= 1'b1;  // 데이터 NACK 오류 기록
            r_phase <= 0;
            r_state <= stop_;
        end
        stop_: begin
            if(tick_i2c) begin
                case(r_phase)
                    0: begin
                        r_scl_low <= 1'b1;
                        r_phase <= 1;
                    end
                    1: begin
                        // SCL LOW에서 SDA를 LOW로 준비
                        r_sda_low <= 1'b1;
                        r_phase <= 2;
                    end
                    2: begin
                        // SDA LOW를 유지하며 SCL 해제
                        r_scl_low <= 1'b0;
                        r_phase <= 3;
                    end
                    3: begin
                        r_phase <= 4;
                    end
                    4: begin
                        // SCL HIGH에서 SDA 해제: STOP 발생
                        r_sda_low <= 1'b0;
                        r_phase <= 5;
                    end
                    5: begin
                        r_phase <= 6;
                    end
                    6: begin
                        r_phase <= 0;
                        r_state <= complete;
                    end
                endcase
            end
        end
        post_byte: begin
            if(r_ctrl[1]) begin
                // 이번 바이트가 전송의 마지막
                r_phase <= 0;
                r_state <= stop_;
            end else if(tick_i2c) begin
                case(r_phase)
                    0: begin
                        // 응답 클럭의 HIGH 구간을 끝냄
                        r_scl_low <= 1'b1;
                        r_phase <= 1;
                    end
                    1: begin
                        // SCL이 LOW가 된 뒤 SDA 해제
                        r_sda_low <= 1'b0;
                        r_phase <= 0;
                        r_state <= complete;
                    end
                endcase
            end
        end
        complete: begin
            status[2] <= 1'b0;  // busy=0
            status[1] <= 1'b1;  // done=1
            r_state <= wait_req;
        end
        read_resp: begin
            if(tick_i2c) begin
                case(r_phase)
                    0: begin
                        r_scl_low <= 1'b1;
                        r_phase <= 1;
                    end
                    1: begin
                        // STOP_EN=0: ACK, LOW로 구동
                        // STOP_EN=1: NACK, SDA 해제
                        r_sda_low <= ~r_ctrl[1];
                        r_phase <= 2;
                    end
                    2: begin
                        r_scl_low <= 1'b0;
                        r_phase <= 3;
                    end
                    3: begin
                        r_phase <= 0;
                        r_state <= post_byte;
                    end
                endcase
            end
        end
        latch: begin
            if(r_ctrl[2]) r_state <= start_;
            else r_state <= route_rw;
        end
        default: begin
            r_state <= wait_req; status <= 3'b000;
            r_sda_low <= 1'b0; r_scl_low <= 1'b0;
        end
        endcase
    end
end
endmodule
