`timescale 1ns / 1ps

module top_btn_debounce(input clk, input reset, input btn_c, output led0);

    wire w_clean_btn;

    button_debounce u_button_debounce(.clk(clk), .reset(reset), .btn(btn_c), .clean_btn(w_clean_btn));

    led_toggle u_led_toggle(.clean_btn(w_clean_btn), .led(led0));

endmodule
