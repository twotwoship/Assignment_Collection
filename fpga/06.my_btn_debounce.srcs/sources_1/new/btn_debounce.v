`timescale 1ns / 1ps
//btn 00 둘다안눌림, btn 01 다운눌림 btn 10 위 눌림 btn 11 둘다눌림
// 매 상승엣지클럭마다 값을 확인해서 같은 값이 10개일 경우에만 확정.
module btn_debounce(input clk, input [1:0]btn, input reset, output [1:0]o_btn);

    reg [1:0]r_o_btn;
    reg [19:0]buff;

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            buff <= 20'b0;
            r_o_btn <= 2'b00;
        end 
        else begin
            // 새로운 2bit 입력 저장
            buff <= {buff[17:0], btn};

            if({buff[17:0], btn} == {10{2'b00}}) r_o_btn <= 2'b00;

            else if({buff[17:0], btn} == {10{2'b01}}) r_o_btn <= 2'b01;

            else if({buff[17:0], btn} == {10{2'b10}})  r_o_btn <= 2'b10;

            else if({buff[17:0], btn} == {10{2'b11}}) r_o_btn <= 2'b11;
        end
    end

    assign o_btn = r_o_btn;
endmodule
