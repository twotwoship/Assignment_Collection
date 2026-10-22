`timescale 1ns / 1ps
module tb_fms_7(  );
    reg clk; reg reset; reg btnU; reg btnD; wire [15:0]led;
    top_fsm u_top_fsm(.clk(clk), .reset(reset), .btnU(btnU), .btnD(btnD), .led(led));
    always #5 clk = ~clk;
    initial begin
        clk = 0; reset = 0; btnU = 0; btnD = 0;
        #11_000_000 reset = 1;
        #11_000_000 reset = 0;
        #11_000_000; btnU = 1; btnD = 0;
        #11_000_000; btnU = 0; btnD = 1;
        #11_000_000; btnU = 1; btnD = 0;
        #11_000_000; btnU = 0; btnD = 1;
        #11_000_000; btnU = 1; btnD = 0;
        #11_000_000; btnU = 0; btnD = 0;
        #11_000_000; btnU = 1; btnD = 0;
        #11_000_000; btnU = 0; btnD = 0;
        #11_000_000; btnU = 1; btnD = 0;
        #11_000_000; btnU = 0; btnD = 0;
        #11_000_000; btnU = 0; btnD = 0;
        #11_000_000 $finish;
    end
endmodule
