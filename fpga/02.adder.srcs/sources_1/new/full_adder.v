`timescale 1ns / 1ps

module full_adder(input wire a, b, cin, output sum, cout);

    wire w_sum1, w_carry_out1, w_carry_out2;
    half_header u1(.a(a), .b(b), .sum(w_sum1), .c_out(w_carry_out1));

    half_header u2(.a(w_sum1), .b(cin), .sum(sum), .c_out(w_carry_out2));

    assign cout = w_carry_out1 | w_carry_out2;

endmodule

