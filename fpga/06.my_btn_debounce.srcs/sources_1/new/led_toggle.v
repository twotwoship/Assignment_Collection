`timescale 1ns / 1ps

module led_toggle(input [1:0]btn, input reset, output led);

    reg r_led;
    
    wire btn_event;

    assign btn_event = (btn == 2'b01);

    always @(posedge btn_event or posedge reset) begin
        if(reset == 1) begin
            r_led <= 1'b0;
        end else begin
            r_led <= ~r_led;
        end
    end

    assign led = (r_led == 1) ? 1'b1 : 1'b0;
endmodule
    