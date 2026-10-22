`timescale 1ns / 1ps

module my_btn_debounce(input clk, input reset, input btn_u, input btn_d, input sel, 
                        output JC3, output led0, output led2);

    wire w_JC3; wire w_led0; wire w_led2;

    wire w_tick_btn; wire [1:0]w_o_mux; wire [1:0]w_o_btn;

    tick_generator u_tick_generator(.clk(clk), .reset(reset), .o_clk(w_tick_btn));

    mux u_mux(.btn_u(btn_u), .btn_d(btn_d), .sel(sel), .o_mux(w_o_mux));

    btn_debounce u_btn_debounce(.clk(w_tick_btn), .btn(w_o_mux), .reset(reset), .o_btn(w_o_btn));

    assign JC3 = w_o_btn[1];
    assign led0 = w_o_btn[1];

    led_toggle u_led_toggle(.btn(w_o_btn), .reset(reset), .led(led2));


endmodule
