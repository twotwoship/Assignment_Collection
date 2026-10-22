`timescale 1ns / 1ps

module tb_btn_debounce( );
    parameter CLK_FREQ = 100000000; // 100Mhz
    parameter CLK_PERIOD = 10;      // 10ns

    //----sim time period setting
    // real board div_time : DIV = 1250000 / 1cycle 12.5ms 80hz
    // sim        div_time : DIV = 100     / 1cycle 1us 1Mhz

    parameter simulation_div = 100;
    localparam sample_period = simulation_div * CLK_PERIOD; // 100 * 10 = 1000ns 1us
    
    // set time of pressing button 
    // sample clk set pressing on 3period
    parameter BTN_PRESS_LIMIT = sample_period * 3; // 3us

    // clock 80hz div_period has coverd sim div_period
    defparam top_btn_debounce.u_button_debounce.u_clock_80hz.DIV = simulation_div;

    reg clk; reg reset; reg btn_c; wire led0;
    top_btn_debounce u_top_btn_debounce(.clk(clk), .reset(reset), .btn_c(btn_c), .led0(led0));

    initial begin
        clk = 0;
        forever begin
            #(CLK_PERIOD/2) clk = ~clk;
        end
    end
    initial begin
        reset = 1;
        btn_c = 0;
        #100;
        reset = 0;
        // btn input
        $display("[%0t] start noise sig generation .................", $time);
        #100; btn_c = 1;
        #200; btn_c = 0;
        #300; btn_c = 1;
        #120; btn_c = 0;
        #500;           //noise pulse
        #100; btn_c = 1;
        # (BTN_PRESS_LIMIT); 
        #100;
        if(led0 == 1'b1) begin
            $display("[%0t] SIM TEST PASSED !!!!!!!!!........",$time);
        end
        else begin
            $display("[%0t] SIM TEST FAILED !!!!!!!!!........",$time);
        end
        #1000; $display("[%0t] SIM TEST finish !!!!!!!!!........",$time);
        $finish;
    end
endmodule
