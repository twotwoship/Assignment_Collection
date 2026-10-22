`timescale 1ns / 1ps

module adder4(input[6:0]a, input[6:0]b, input cin, output[6:0]sum, output carry_out);

    wire [5:0]carry_out_in;
    full_adder FA1(.a(a[0]), .b(b[0]), .cin(cin), .sum(sum[0]), .cout(carry_out_in[0]));
    full_adder FA2(.a(a[1]), .b(b[1]), .cin(carry_out_in[0]), .sum(sum[1]), .cout(carry_out_in[1]));
    full_adder FA3(.a(a[2]), .b(b[2]), .cin(carry_out_in[1]), .sum(sum[2]), .cout(carry_out_in[2]));
    full_adder FA4(.a(a[3]), .b(b[3]), .cin(carry_out_in[2]), .sum(sum[3]), .cout(carry_out_in[3]));
    full_adder FA5(.a(a[4]), .b(b[4]), .cin(carry_out_in[3]), .sum(sum[4]), .cout(carry_out_in[4]));
    full_adder FA6(.a(a[5]), .b(b[5]), .cin(carry_out_in[4]), .sum(sum[5]), .cout(carry_out_in[5]));
    full_adder FA7(.a(a[6]), .b(b[6]), .cin(carry_out_in[5]), .sum(sum[6]), .cout(carry_out));

endmodule
