`timescale 1ns / 1ps
module top_moor_fsm(input clk, input reset, input btnU, input btnD, output led);

wire w_clean_btnU;
wire w_clean_btnD;

reg r_prev_btnU;
reg r_prev_btnD;

wire w_btnU;
wire w_btnD;

debouncer u_debouncer(.clk(clk), .reset(reset), .noisy_btn(btnU), .clean_btn(w_clean_btnU));

debouncer u_debouncer1(.clk(clk), .reset(reset), .noisy_btn(btnD), .clean_btn(w_clean_btnD));

//버튼 값 저장
always @(posedge clk or posedge reset) begin
    if(reset) begin
        r_prev_btnU <= 0;
        r_prev_btnD <= 0;
    end
    else begin
        r_prev_btnU <= w_clean_btnU;
        r_prev_btnD <= w_clean_btnD;
    end
end

assign w_btnU = w_clean_btnU & ~r_prev_btnU;
assign w_btnD = w_clean_btnD & ~r_prev_btnD;

moor_fsm_101 u_moor_fsm_101(.clk(clk), .reset(reset), .x(w_btnU), .x1(w_btnD), .y(led));

endmodule
