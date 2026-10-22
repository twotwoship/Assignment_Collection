`timescale 1ns / 1ps

module button_debounce(input clk, input reset, input btn, output clean_btn);

    wire w_o_clk;
    wire w_Q1;
    wire w_Q2; wire w_Q2_bar;

    clock_80hz u_clock_80hz(.clk(clk), .reset(reset), .o_clk(w_o_clk));

    
    D_FF u1_D_FF(.clk(w_o_clk), .reset(reset), .D(btn), .Q(w_Q1));

    
    D_FF u2_D_FF(.clk(w_o_clk), .reset(reset), .D(w_Q1), .Q(w_Q2));

    assign w_Q2_bar = ~w_Q2;
    assign clean_btn = w_Q1 & w_Q2_bar;
    
endmodule
