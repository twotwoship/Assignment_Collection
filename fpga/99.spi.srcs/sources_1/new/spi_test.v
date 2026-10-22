/*
19	MOSI 입력 21	MISO 출력 23	SCLK 입력 24	CS0	 입력 25	GND	GND
*/
`timescale 1ns / 1ps

module spi_test (
    input clk,    input reset,    input spi_sclk,    input spi_cs_n,    input spi_mosi,
    output wire spi_miso,    input [7:0] tx_data,    output reg [7:0] rx_data,    output reg rx_valid
);
    (* ASYNC_REG = "TRUE" *) reg [1:0] sclk_sync;
    (* ASYNC_REG = "TRUE" *) reg [1:0] cs_sync;
    (* ASYNC_REG = "TRUE" *) reg [1:0] mosi_sync;
    reg sclk_prev;

    wire sclk_rising = sclk_sync[1] & ~sclk_prev;
    wire sclk_falling = ~sclk_sync[1] & sclk_prev;
    reg [7:0] rx_shift;    reg [7:0] tx_shift;
    reg [2:0] bit_count;

    // Preload while deselected so the first MSB is ready before SCLK.
    // Release the shared MISO line immediately when CS is inactive.
    assign spi_miso = (spi_cs_n || reset) ? 1'bz : tx_shift[7];

    always @(posedge clk) begin
        if (reset) begin
            sclk_sync <= 2'b00; cs_sync <= 2'b11;
            mosi_sync <= 2'b00; sclk_prev <= 1'b0;
        end else begin
            sclk_sync <= {sclk_sync[0], spi_sclk};
            cs_sync <= {cs_sync[0], spi_cs_n};
            mosi_sync <= {mosi_sync[0], spi_mosi};
            sclk_prev <= sclk_sync[1];
        end
    end

    always @(posedge clk) begin
        if (reset) begin
            rx_shift <= 8'h00; tx_shift <= 8'h00;
            bit_count <= 3'd0;
            rx_data <= 8'h00; rx_valid <= 1'b0;
        end else begin
            rx_valid <= 1'b0;
            if (cs_sync[1]) begin
                rx_shift <= 8'h00;
                bit_count <= 3'd0;
                tx_shift <= tx_data;
            end else begin
                // No separate CS-start branch: a simultaneous detected
                // CS assertion and SCLK rising edge must still sample MOSI.
                if (sclk_rising) begin
                    rx_shift <= {rx_shift[6:0], mosi_sync[1]};
                    bit_count <= bit_count + 3'd1;
                    if (bit_count == 3'd7) begin
                        rx_data <= {rx_shift[6:0], mosi_sync[1]};
                        rx_valid <= 1'b1;
                    end
                end
                if (sclk_falling) begin
                    // The preceding eighth rising edge wrapped the count.
                    if (bit_count == 3'd0)
                        tx_shift <= tx_data;
                    else
                        tx_shift <= {tx_shift[6:0], 1'b0};
                end
            end
        end
    end
endmodule
