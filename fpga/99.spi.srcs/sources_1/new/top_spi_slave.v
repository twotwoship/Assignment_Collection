`timescale 1ns / 1ps

module top_spi (
    input clk,    input reset,
    input spi_sclk,    input spi_cs_n,    input spi_mosi,    output spi_miso,
    output [7:0] led
);
    wire [7:0] rx_data;    wire       rx_valid;

    spi_test u_spi_test (
        .clk      (clk),        .reset(reset),
        .spi_sclk (spi_sclk),   .spi_cs_n (spi_cs_n),   .spi_mosi (spi_mosi),  .spi_miso (spi_miso),
        .tx_data  (8'h55),  // 전송할 데이터
        .rx_data  (rx_data),        .rx_valid (rx_valid)
    );

    assign led = rx_data;
endmodule