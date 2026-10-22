`timescale 1ns / 1ps

module tb_mux4_1();
    //input output init
    reg [3:0]a; reg [3:0]b; reg sel;
    wire [3:0]out; wire [7:0]led;

    // module dut instance
    mux4_1 u_mux4_1( .a(a), .b(b), .sel(sel), .out(out), .led(led) );

    // test script
    initial begin
        //init
        a = 0; b = 0; sel = 0;
        // result display
        $monitor("Time=%0t | sel = %b | a = %b | b = %b |out = %b", $time, sel,a,b,out);

        // if sel is 0, out is b
        #10; sel = 0; a=4'b1000; b=4'b0000;
        #10; sel = 0; a=4'b0100; b=4'b0001;
        #10; sel = 0; a=4'b0010; b=4'b0010;
        #10; sel = 0; a=4'b0001; b=4'b0100;
        #10; sel = 0; a=4'b0000; b=4'b1000;

        // if sel is 1, out is a
        #10; sel = 1; a=4'b0000; b=4'b1000;
        #10; sel = 1; a=4'b0001; b=4'b0100;
        #10; sel = 1; a=4'b0010; b=4'b0010;
        #10; sel = 1; a=4'b0100; b=4'b0001;
        #10; sel = 1; a=4'b1000; b=4'b0000;

        // add all
        #10; sel = 0; a=4'b0100; b=4'b0001;
        #10; sel = 1; a=4'b1000; b=4'b0000;
        #10; $finish;

    end
endmodule
