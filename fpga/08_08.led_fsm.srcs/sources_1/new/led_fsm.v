`timescale 1ns / 1ps

module led_fsm(input clk, input reset, input btnC, output [4:0]led);

    wire w_clean_btn;
    reg r_prev_btn;
    wire w_btn;
    btn_2ff_debounce u_btn_2ff_debounce(
    .clk(clk), .reset(reset), .noisy_btn(btnC), .clean_btn(w_clean_btn) );

    always @(posedge clk or posedge reset) begin
        if(reset) begin
            r_prev_btn <= 0;
        end else begin
            r_prev_btn <= w_clean_btn;
        end
    end

    assign w_btn = w_clean_btn & ~r_prev_btn;

    led_mealy_fsm u_led_mealy_fsm(
        .clk(clk), .reset(reset), .clean_btn(w_btn), .led(led) );

endmodule
