`timescale 1ns / 1ps

module top_led_(input clk, input reset, output led1);

    wire w_clk;
    wire o_led1;
    tick_generator u_tick_generator(clk, reset, w_clk);

    led_on_off_500ms u_led_on_off_500ms(.i_clk(w_clk), .o_led(o_led1));

    assign led1 = o_led1;
endmodule
