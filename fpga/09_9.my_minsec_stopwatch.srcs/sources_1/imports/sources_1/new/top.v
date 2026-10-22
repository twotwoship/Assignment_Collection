`timescale 1ns / 1ps

module top #(parameter NS_LIMIT = 1_000_000)(input clk, reset, [2:0]btn, output [7:0]seg, [3:0]an);

    wire [2:0]w_clean_btn;
    wire [23:0]w_seg_data;
    wire w_tick;
    wire w_mode_sel;
    wire w_mode2_sel;
    btn_debounder u_btn_debounder(.clk(clk), .reset(reset), .noisy_btn(btn), .clean_btn(w_clean_btn));
    main_fsm #(.NS_LIMIT(NS_LIMIT))u_main_fsm(.clk(clk), .reset(reset), .btn(w_clean_btn), .seg(w_seg_data), .mode_sel(w_mode_sel), .mode2_sel(w_mode2_sel));
    tick_generator u_tick_generator(.clk(clk), .reset(reset), .tick(w_tick));
    fnd_controllor #(.NS_LIMIT(NS_LIMIT))u_fnd_controllor(.clk(clk), .reset(reset), .tick(w_tick), .mode_sel(w_mode_sel), .mode2_sel(w_mode2_sel), .in_data(w_seg_data), .an(an), .seg(seg));

endmodule
