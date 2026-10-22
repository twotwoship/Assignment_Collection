`timescale 1ns / 1ps

module p_encoder( input[3:0]a, output[1:0]led, output v );

    reg [1:0] r_led; reg r_v;

    always @(*) begin
        r_v = 1'b1;
        if(a[3] == 1'b1) r_led = 2'b11;
        else if(a[2] == 1'b1) r_led = 2'b10;
        else if(a[1] == 1'b1) r_led = 2'b01;
        else if(a[0] == 1'b1) r_led = 2'b00;
        else begin
            r_led = 2'b00;
            r_v = 1'b0;   
        end

    end
    assign led = r_led;
    assign v = r_v;
endmodule


