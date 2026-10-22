`timescale 1ns / 1ps


module tb_half_adder();
    reg i_a, i_b;
    wire o_sum, o_c_out;

    half_header u_half_header(
        .a(i_a),
        .b(i_b), 
        .sum(o_sum), 
        .c_out(o_c_out)
        );

        initial begin
            #00 i_a = 1'b0; i_b = 1'b0;
            #10 i_a = 1'b0; i_b = 1'b1;
            #10 i_a = 1'b1; i_b = 1'b0;
            #10 i_a = 1'b1; i_b = 1'b1;
            #10 $finish;
        end
    


endmodule
