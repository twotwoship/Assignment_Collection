`timescale 1ns / 1ps

module add_sub4( input[3:0]a, input[3:0]b, input add_nsub, output add_sub_out, 
output carry_out, output [3:0]sum, output [3:0]leda, output [3:0]ledb );

    reg r_carry_out; reg [3:0]r_sum; reg [3:0]r_ledb; reg [3:0]r_leda;

    always @(*) begin
        if(add_nsub == 1'b1) {r_carry_out, r_sum} = a[3:0] + b[3:0];
        else {r_carry_out, r_sum} = {1'b0, a} + {1'b0, ~b} + 5'b00001;
    end

    assign leda = a;
    assign ledb = b;
    assign carry_out = r_carry_out;
    assign sum = r_sum;
    assign add_sub_out= add_nsub;
endmodule
