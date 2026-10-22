`timescale 1ns / 1ps

/*
module gates(input a, b, output ld0, ld1, ld2, ld3, ld4, ld5);
    assign ld0 = a & b;
    assign ld1 = a | b;
    assign ld2 = ~(a & b);  // NAND
    assign ld3 = ~(a | b);  // NOR
    assign ld4 = a ^ b;     // XOR
    assign ld5 = ~a;        // NOT

endmodule
*/

module gates(input wire [2:0]a, output wire [5:0]led);
    assign led[0] = a[1] & a[0];
    assign led[1] = a[1] | a[0];
    assign led[2] = ~(a[1] & a[0]);  // NAND
    assign led[3] = ~(a[1] | a[0]);  // NOR
    assign led[4] = a[1] ^ a[0];     // XOR
    assign led[5] = ~a[1];        // NOT

endmodule