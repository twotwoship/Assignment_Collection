`timescale 1ns / 1ps
            // down 0 up 0   00 <=무시
            // down 0 up 1   01 <=숫자키워잇
            // down 1 up 0   10 <=숫자줄여잇
            // down 1 up 1   11 <=무시
            // 10ms 마다 해야되니까 100mhz인데 10ns인데 1000ns가 1us 1000us가 1ms 10ms
            // 100clk이 1us 100clk * 1000이 1ms 100 * 1000 * 10이 ms 1000000ns가 10ms
module up_counter #(parameter DIV = 1000000)(input clk, input reset, input[1:0]in_bit, output [15:0]led);
    reg [15:0]o_led;
    reg [1:0] pre_in_bit;
    reg [$clog2(DIV) - 1:0] r_counter = 0;
    always @(posedge clk or posedge reset) begin
        if(reset) begin
            o_led <= 16'b0;
            pre_in_bit <= 2'b0;
            r_counter <= 0;
        end
        else begin
            if(r_counter == DIV - 1) begin
                r_counter <= 0;
                if(pre_in_bit != in_bit) begin
                pre_in_bit <= in_bit;
                if(in_bit == 2'b01) o_led <= o_led + 1;
                else if(in_bit == 2'b10) begin
                    if(o_led != 0) o_led <= o_led - 1;    
                    end
                end
            end
            else begin
                r_counter <= r_counter + 1;
            end
        end           
    end
    assign led = o_led;
endmodule
