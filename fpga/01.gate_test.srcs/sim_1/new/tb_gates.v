`timescale 1ns / 1ps

module tb_gates();
    reg [2:0] i_a;    
    wire [5:0] o_led;

    gates u_gates(
        .a(i_a), .led(o_led)
    );

    initial begin
        #00 i_a[0]=1'b0; i_a[1]=1'b0; 
        #20 i_a[0]=1'b0; i_a[1]=1'b1; 
        #20 i_a[0]=1'b1; i_a[1]=1'b0; 
        #20 i_a[0]=1'b1; i_a[1]=1'b1; 
        #20 i_a[0]=1'b0; i_a[1]=1'b0; 
        #20 $stop;

    end

endmodule
