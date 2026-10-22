`timescale 1ns / 1ps

module mux4_1(input [3:0]a, input [3:0]b, input sel, output [3:0]out, output [7:0]led);
    integer i;
    reg [3:0]r_out;
    assign led[7:4] =  b;
    assign led[3:0] =  a;
    always @(*) begin
            for(i = 0; i < 4; i = i + 1) begin
                case (sel)
                    1'b1 : r_out[i] = a[i];
                    1'b0 : r_out[i] = b[i];
                    default : r_out = 4'bxxxx;        
                endcase
            end
    end
    assign out = r_out;
endmodule
