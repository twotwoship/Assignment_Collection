`timescale 1ns / 1ps

module tick_generator(input clk, input reset, output reg o_clk );

    reg [$clog2(100000) - 1:0] r_counter = 0;

    always @(posedge clk, posedge reset) begin
        if(reset == 1) begin
            o_clk <= 0;
            r_counter <= 0;
        end
        else begin
            if(r_counter == (100000/2) - 1 ) begin
                r_counter <= 0;
                o_clk <= ~o_clk;
            end
            else begin
                r_counter <= r_counter + 1;
            end
        end
    end
endmodule

