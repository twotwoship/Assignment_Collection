`timescale 1ns / 1ps
// 기존 100mhz 에서 1mhz로 바꾸기 
module tick_generator #(parameter DIV = 100000)(input clk, input reset, output reg o_clk);

reg [$clog2(DIV) - 1:0] r_counter = 0;
always @(posedge clk, posedge reset)begin
    if(reset == 1) begin
        o_clk <= 0;
        r_counter <= 0;
    end
    else begin
        if(r_counter == (DIV/2) - 1) begin
            r_counter <= 0;
            o_clk <= ~o_clk;
        end else begin
            r_counter <= r_counter + 1;
        end
    end
end
endmodule
