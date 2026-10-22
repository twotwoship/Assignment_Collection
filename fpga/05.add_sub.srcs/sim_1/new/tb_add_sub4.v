`timescale 1ns / 1ps

module tb_add_sub4();
    wire t_add_sub_out; wire t_carry_out; wire [3:0]t_sum; wire[3:0]t_leda; wire [3:0]t_ledb;
    reg [3:0]t_a; reg [3:0]t_b; reg t_add_nsub;
    
    add_sub4 u_add_sub4(.a(t_a), .b(t_b), .add_nsub(t_add_nsub), .add_sub_out(t_add_sub_out), 
    .carry_out(t_carry_out), .sum(t_sum), .leda(t_leda), .ledb(t_ledb));

    initial begin
    t_a = 4'b0000;
    t_b = 4'b0000;
    t_add_nsub = 1'b1;

    #10; t_add_nsub = 1'b1; t_a = 4'b0010; t_b = 4'b0001;
    #10; t_add_nsub = 1'b0; t_a = 4'b0000; t_b = 4'b0000; 
    #10; t_add_nsub = 1'b0; t_a = 4'b0100; t_b = 4'b0001; 
    #10; t_add_nsub = 1'b0; t_a = 4'b0010; t_b = 4'b0100; 

    #10; $finish;
    end

endmodule
