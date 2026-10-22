`timescale 1ns / 1ps

module top(input clk, reset, [2:0]btn, [7:0]sw, output [7:0]seg, [3:0]an, [15:0]led);

    wire [2:0]w_clean_btn;
    wire [13:0]w_seg_data;
    wire w_tick;
    btn_debounder u_btn_debounder(.clk(clk), .reset(reset), .noisy_btn(btn), .clean_btn(w_clean_btn));

    main_controllor u_main_controllor(.clk(clk), .reset(reset), .btn(w_clean_btn), .sw(sw), .seg(w_seg_data), .led(led));

    tick_generator u_tick_generator(.clk(clk), .reset(reset), .tick(w_tick));

    fnd_controllor u_fnd_controllor(.clk(clk), .reset(reset), .tick(w_tick), .in_data(w_seg_data), .an(an), .seg(seg));

endmodule
