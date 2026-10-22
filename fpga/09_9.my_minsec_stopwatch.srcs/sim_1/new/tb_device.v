`timescale 1ns / 1ps
module tb_device();
    reg clk, reset; reg [2:0] btn; wire [7:0] seg; wire [3:0] an;
    parameter NONE = 3'b000; parameter btnL = 3'b001; parameter btnC = 3'b010; parameter btnR = 3'b100;
    top #(.NS_LIMIT(10000)) u_top ( .clk(clk), .reset(reset), .btn(btn), .seg(seg), .an(an) );
    always #5 clk = ~clk;
    task press_btn(input [2:0] b); begin #11_000_000; btn = b; #11_000_000; btn = NONE; end
    endtask
    initial begin
        clk = 0; reset = 0; btn = NONE;
        // reset
        #11_000_000; reset = 1; #11_000_000; reset = 0;
        press_btn(btnL);   // CLOCK -> STOPWATCH
        #100_00000;     press_btn(btnC);   // STOPWATCH -> RESET
        #100_00000;     press_btn(btnC);   // RESET -> STOPWATCH
        #100_00000;     press_btn(btnR);   // STOPWATCH -> STOP
        #100_000000;    press_btn(btnR);   // STOP -> STOPWATCH
        #100_00000;     press_btn(btnL);   // STOPWATCH -> CLOCK
        #100_00000; $finish;
    end
endmodule

