`timescale 1ns / 1ps

module half_header(input wire a, input wire b, output sum, c_out);
    assign sum = a^b;
    assign c_out = a & b;
endmodule
