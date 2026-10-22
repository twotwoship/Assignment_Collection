`timescale 1ns / 1ps
module mux(input btn_u, input btn_d, input sel, output reg [1:0]o_mux);
    always @(*) begin
        if(sel == 1'b1) begin
            if(!btn_d && btn_u) o_mux = 2'b10;
            else if(btn_d && !btn_u) o_mux = 2'b01;
            else if(!btn_d && !btn_u) o_mux = 2'b00;
            else o_mux = 2'b11;
        end else begin
            o_mux = 2'b00;
        end
    end
endmodule
