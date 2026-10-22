`timescale 1ns / 1ps

module tb_adder7();
    reg[6:0]a;  reg[6:0]b; reg cin; wire[6:0]sum;  wire carry_out;
    adder4 dut(.a(a), .b(b), .cin(cin), .sum(sum), .carry_out(carry_out));
    initial begin
        #00 a=0; b=0; cin=0;
        #10 a=0; b=2;
        #10 a=19; b=110;
        #10 a=27; b=48;
        #10 a=45; b=24;
        for(integer i = 0; i < 20; i = i + 1) begin
            #10 a=i;
        end
        #10 $finish;        
    end
endmodule
