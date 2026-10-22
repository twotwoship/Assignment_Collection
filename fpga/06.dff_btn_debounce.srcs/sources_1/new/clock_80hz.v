`timescale 1ns / 1ps
// 100mhz -> 80hz
module clock_80hz #(parameter DIV = 1250000)(input clk, input reset, output reg o_clk);
    // parameter sim override
    //reg [23:0] r_counter = 0; // rectangle pulse making, 12.5ms
    reg [$clog2(DIV) - 1:0] r_counter = 0; // rectangle pulse making, 12.5ms macro size cul
    always @(posedge clk, posedge reset) begin
        if(reset == 1) begin
            o_clk <= 0;
            r_counter <= 0;
        end
        else begin
            if(r_counter == (DIV/2) -1 ) begin
                r_counter <= 0;
                o_clk <= ~o_clk;
            end else begin
                r_counter <= r_counter + 1;    
            end
        end
    end    
endmodule
