`timescale 1ns / 1ps

module led_on_off_500ms(input i_clk, output o_led);

reg [$clog2(500) - 1 : 0]tick_counter = 0;
reg r_switch = 1'b0;
always @(posedge i_clk) begin
    if(tick_counter > 500 -1 ) begin
        tick_counter <= 0;
        r_switch <= ~r_switch;
    end
    else begin
        tick_counter <= tick_counter + 1;
    end
end
assign o_led = (r_switch == 1) ? 1'b1 : 1'b0;
endmodule
