`timescale 1ns / 1ps

module tb_mux2_1();
    //input output init
    reg a, b, sel;
    wire out;

    // module dut instance
    mux2_1 u_mux2_1( .a(a), .b(b), .sel(sel), .out(out) );

    // test script
    initial begin
        //init
        a = 0; b = 0; sel = 0;
        // result display
        $monitor("Time=%0t | sel = %b | a = %b | b = %b |out = %b", $time, sel,a,b,out);

        // if sel is 0, out is b
        #10; sel = 0; a=1; b=0;
        #10; sel = 0; a=0; b=1;

        // if sel is 1, out is a
        #10; sel = 1; a=1; b=0;
        #10; sel = 1; a=0; b=1;

        // add all
        #10; sel = 0; a=1; b=1;
        #10; sel = 1; a=1; b=1;
        #10; $finish;

    end
endmodule
