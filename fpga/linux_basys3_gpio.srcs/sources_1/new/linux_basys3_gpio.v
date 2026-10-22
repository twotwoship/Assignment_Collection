`timescale 1ns / 1ps

module linux_basys3_gpio(input wire [2:0]_sw, output [1:0]_gpio);
    assign _gpio[0] = _sw[0] ^ _sw[1];
    assign _gpio[1] = _sw[1] & _sw[2];
endmodule

// jetson gpio 21 23
