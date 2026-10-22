`timescale 1ns / 1ps

module tb_incoder();
    reg [3:0] r_a; wire [1:0] w_led;

    incoder u_incoder(.a(r_a), .led(w_led));

    initial begin
        r_a = 4'b0000;

        #10; r_a = 4'b0001;
        #10; r_a = 4'b0010;
        #10; r_a = 4'b0100;
        #10; r_a = 4'b1000;
        #10; $finish;
    end


endmodule
