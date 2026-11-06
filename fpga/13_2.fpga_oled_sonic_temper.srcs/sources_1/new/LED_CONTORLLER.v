`timescale 1ns / 1ps

module LED_CONTOLLER(input clk, reset, [15:0]in_led, output reg [15:0]out_led);

always @(posedge clk or posedge reset) begin
    if(reset) begin
        out_led <= 16'h0000;
    end else begin
        out_led <= in_led;
    end
end

endmodule