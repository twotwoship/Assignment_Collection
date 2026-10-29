`timescale 1ns / 1ps
module shift_reg(input clk, input reset, input btn_press, input x, output [15:0]led);

// when catch out button, reg shift
    reg [6:0] sr3;
    always@(posedge clk, posedge reset) begin
        if(reset) begin
            sr3 <= 7'b0000000;
        end else if(btn_press)begin
            sr3 <= {sr3[5:0], x};
        end
    end

    assign led[6:0] = sr3;
    assign led[14:7] = 8'b0;
    assign led[15] = (sr3 == 7'b1010111) ? 1'b1 : 1'b0;

endmodule
