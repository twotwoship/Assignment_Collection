`timescale 1ns / 1ps

module incoder( input[3:0]a, output[1:0]led   );

    reg [1:0] r_led;

    always @(*) begin

        if(a[3] == 1'b1) r_led = 2'b11;
        else if(a[2] == 1'b1) r_led = 2'b10;
        else if(a[1] == 1'b1) r_led = 2'b01;
        else r_led = 2'b00;

        // case (a)
        //     4'b1000: r_led = 2'b00;
        //     4'b0100: r_led = 2'b01;
        //     4'b0010: r_led = 2'b10;
        //     4'b0001: r_led = 2'b11;
        // endcase
    end
    assign led = r_led;
endmodule

