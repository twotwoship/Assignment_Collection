`timescale 1ns / 1ps
module btn_debounce(input clk, input reset, input btn_u, input btn_d, output [1:0]out_bit);
    reg [1:0]r_o_btn;    wire [1:0]btn_event = {btn_d, btn_u};    reg [19:0] buff;
    always @(posedge clk or posedge reset) begin
        if(reset) begin            buff <= 20'b0;            r_o_btn <= 2'b00;        end
        else begin
            buff <= {buff[17:0], btn_event};
            // down 0 up 0
            if({buff[17:0], btn_event} == {10{2'b00}}) r_o_btn <= 2'b00;
            // down 0 up 1
            else if({buff[17:0], btn_event} == {10{2'b01}}) r_o_btn <= 2'b01;
            // down 1 up 0
            else if({buff[17:0], btn_event} == {10{2'b10}}) r_o_btn <= 2'b10;
            // down 1 up 1
            else if({buff[17:0], btn_event} == {10{2'b11}}) r_o_btn <= 2'b11;
        end 
    end
    assign out_bit = r_o_btn;
endmodule
