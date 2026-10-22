`timescale 1ns / 1ps
            // down 0 up 0   00 <= 무시
            // down 0 up 1   01 <=
            // down 1 up 0   10 <=
            // down 1 up 1   11 <= 무시
            // 10ms 마다 해야되니까 100mhz인데 10ns인데 1000ns가 1us 1000us가 1ms 10ms
            // 100clk이 1us 100clk * 1000이 1ms 100 * 1000 * 10이 ms 1000000ns가 10ms
module up_counter2 #(parameter DIV = 1000000)(input clk, input reset, input[1:0]in_bit, output [15:0]led);

    localparam RUN = 1'b1;
    localparam STOP = 1'b0;
    reg [15:0]o_led;
    reg [1:0] pre_in_bit;
    reg in_state;
    reg [$clog2(DIV) - 1:0] r_counter = 0; //10ms마다 커져야됨
    /*
    reset이 아닐땐 
    */

    always @(posedge clk or posedge reset) begin
        if(reset) begin
            o_led <= 16'b0;
            pre_in_bit <= 2'b0;
            r_counter <= 0;
            in_state <= RUN;
        end
        else
                case (in_state) //상태에 따른 버튼 동작
                RUN: begin // 돌아가고 있을 때는 1. 10ms 마다 o_led(값)가 커져야됨
                //2. in_bit가 01이면 stop상태로 들어가기
                    if(pre_in_bit != in_bit) begin // 이전 값과 다를때만 동작 같으면 안함.
                        pre_in_bit <= in_bit;   
                        if(in_bit == 2'b01) in_state <= STOP;
                    end
                    if(r_counter == DIV - 1) begin
                        r_counter <= 0;
                        o_led <= o_led + 1;
                    end
                    else r_counter <= r_counter + 1;
                end

                STOP: begin // 1. in_bit가 01이 들어올때가지 아무일도안함 01이 들어오면 상태를 RUN으로 전환
                // 2. 10이 들어오면 에리디 초기화
                if(pre_in_bit != in_bit) begin // 이전 값과 다를때만 동작 같으면 안함.
                    pre_in_bit <= in_bit;   
                    if(in_bit == 2'b01) in_state <= RUN;
                    else if(in_bit == 2'b10)  begin
                        o_led <= 16'b0;
                        r_counter <= 0;
                    end
                end  
                end
                endcase             
    end
    assign led = o_led;
endmodule

