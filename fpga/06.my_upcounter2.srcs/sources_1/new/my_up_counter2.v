`timescale 1ns / 1ps
module my_up_counter2(input clk, input reset, input btn_u, input btn_d, output[15:0]led);
    wire [1:0]w_bit; 
    btn_debounce u_btn_debounce(.clk(clk), .reset(reset), .btn_u(btn_u), .btn_d(btn_d), .out_bit(w_bit));
    up_counter2 u_up_counter2(.clk(clk), .reset(reset), .in_bit(w_bit), .led(led));
endmodule
