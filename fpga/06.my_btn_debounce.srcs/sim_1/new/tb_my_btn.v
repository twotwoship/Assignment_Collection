`timescale 1ns / 1ps

module tb_my_btn();
    parameter CLK_FREQ = 100000000; // 100Mhz
    parameter CLK_PERIOD = 10;      // 10ns

    localparam MS = 1_000_000;   // 1ms = 1,000,000ns

    reg clk; reg reset; reg btn_u; reg btn_d; reg sel;
    wire JC3; wire led0; wire led2;
    my_btn_debounce u_my_btn_debounce(.clk(clk), .reset(reset), .btn_u(btn_u), .btn_d(btn_d), .sel(sel), 
                        .JC3(JC3), .led0(led0), .led2(led2));
    
    initial begin
        clk = 0;
        forever begin
            #(CLK_PERIOD/2) clk = ~clk;
        end
    end
    initial begin
        reset = 1;        btn_u = 0;        btn_d = 0;
        sel = 0;
        #(20*MS);     reset = 0;        sel = 1;
        #(20*MS);             btn_u = 1;
        #(20*MS);           btn_u = 0;        btn_d = 1;
        #(20*MS);             btn_d = 0;
        #(20*MS);             btn_d = 1;
        #(20*MS);             btn_d = 0;
        #(20*MS);    
        $finish;
    end

endmodule
