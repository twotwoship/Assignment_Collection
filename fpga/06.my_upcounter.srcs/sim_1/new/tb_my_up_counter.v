`timescale 1ns / 1ps



module tb_my_up_counter();
    parameter CLK_FREQ = 100000000; // 100Mhz
    parameter CLK_PERIOD = 10;      // 10ns

    localparam MS = 1_000_000;   // 1ms = 1,000,000ns

    reg clk; reg reset; reg btn_u; reg btn_d;
    wire [15:0]led;
    my_up_counter u_my_up_counter(.clk(clk), .reset(reset), .btn_u(btn_u), .btn_d(btn_d), .led(led));

    initial begin
        clk = 0;
        forever begin
            #(CLK_PERIOD/2) clk = ~clk;
        end
    end

    initial begin
        reset = 1; btn_u = 0; btn_d = 0;
        #(20*MS); reset = 0;
        #(20*MS); btn_u = 1;
        #(20*MS);   btn_u = 0;
        #(20*MS);   btn_u = 1;
        #(20*MS);   btn_u = 0;
        #(20*MS);   btn_u = 1;
        #(20*MS);btn_u = 0;
        #(20*MS);   btn_u = 1;
        #(20*MS);   btn_u = 0;
        #(20*MS);btn_d = 1;
        #(20*MS);   btn_d = 0;
        #(20*MS);   reset = 1;
        #(20*MS);   reset = 0;
        $finish;
    end
endmodule
