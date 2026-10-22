`timescale 1ns / 1ps

module priority_encoder( input[3:0]a, output[1:0]led, output v );

    reg [1:0] r_led; reg r_v;

    always @(*) begin

        if(a[3] == 1'b1){
            r_led = 2'b11;
            r_v = 1 ;
        }
        else if(a[2] == 1'b1) r_led = 2'b10;
        else if(a[1] == 1'b1) r_led = 2'b01;
        else if(a[0] == 1'b1) r_led = 2'b00;
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

