`timescale 1ns / 1ps
/*
입력과 동시에 출력 영향, 한쪽 방향으로만 이동하는거.큰 하나의 사이클
using four led, led[4] = direction, led[3:0] state;
s00 s01 s10  띵킹역방향 하나 reg 줘서 온오프 판단.
*/
module led_mealy_fsm(input clk, input reset, input clean_btn, output [4:0]led);
    localparam [1:0]S0 = 2'b00;  localparam [1:0]S1 = 2'b01; localparam [1:0]S2 = 2'b10;
    reg [1:0] r_curr_state, r_next_state;
    reg r_pre_direction; reg r_direction; // led[4]
    reg [3:0]r_prev_led; reg [3:0]r_led; // led[3:0]

    always@(posedge clk or posedge reset) begin
        if(reset) begin
            r_curr_state <= S0; r_pre_direction <= 0; r_prev_led <= 4'b0001;
        end else begin 
            r_curr_state <= r_next_state; r_pre_direction <= r_direction;
            r_prev_led <= r_led;
        end
    end

    always@(*) begin
        r_next_state = r_curr_state; r_direction = r_pre_direction; r_led = r_prev_led;
        if(r_direction) begin // reverse
            if(clean_btn) begin // btn press
                case(r_curr_state)
                    S0: begin r_next_state = S1; r_direction = 1'b0; r_led = 4'b0010; end
                    S1: begin r_next_state = S0;  r_led = 4'b0001; end
                    S2: begin r_next_state = S1;  r_led = 4'b0010; end
                    default: r_next_state = S0;
                endcase
            end
        end else begin // forword 정방향일때 r_d는 0이고 버튼일 눌릴때마다 상태와 결과가 바뀜
            if(clean_btn) begin // btn press
                case(r_curr_state)
                // 메일리는 출력이 상태와 입력으로 결정됨.
                    S0: begin r_next_state = S1; r_led = 4'b0010; end
                    S1: begin r_next_state = S2; r_led = 4'b0100; end
                    S2: begin // 역방향 전환이 벌써이루어지면되나? 아닌데
                        if(r_led == 4'b1000) begin
                            r_direction = 1'b1;  r_led = 4'b0100;
                        end else begin r_led = 4'b1000; end
                    end
                    default: r_next_state = S0;
                endcase
            end
        end
    end
    assign led = {r_direction, r_led};
endmodule
