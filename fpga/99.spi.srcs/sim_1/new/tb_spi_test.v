`timescale 1ns / 1ps

module tb_spi_test();

    reg clk;
    reg reset;

    reg spi_sclk;
    reg spi_cs_n;
    reg spi_mosi;
    wire spi_miso;

    reg  [7:0] tx_data;
    wire [7:0] rx_data;
    wire rx_valid;

    reg [7:0] received_miso;

    spi_test uut (
        .clk(clk),
        .reset(reset),
        .spi_sclk(spi_sclk),
        .spi_cs_n(spi_cs_n),
        .spi_mosi(spi_mosi),
        .spi_miso(spi_miso),
        .tx_data(tx_data),
        .rx_data(rx_data),
        .rx_valid(rx_valid)
    );

    // 100 MHz
    always #5 clk = ~clk;


    task spi_transfer;

        input  [7:0] send_data;
        output [7:0] recv_data;

        integer i;

        begin

            recv_data = 8'h00;

            // CS 활성화
            spi_cs_n = 1'b0;

            // DUT의 CS synchronizer가 인식할 시간
            #100;

            $display("");
            $display("================================");
            $display(" SPI TRANSFER START");
            $display(" MASTER TX = %b (0x%02h)", send_data, send_data);
            $display("================================");


            for(i = 7; i >= 0; i = i - 1) begin

                //--------------------------------
                // MOSI 준비
                //--------------------------------
                spi_mosi = send_data[i];

                #50;


                //--------------------------------
                // SCLK Rising
                // Slave가 MOSI 수신
                //--------------------------------
                spi_sclk = 1'b1;


                // Master는 MISO 샘플
                #1;
                recv_data[i] = spi_miso;


                // DUT 내부 2FF synchronizer +
                // edge detect가 처리될 시간
                #39;


                $display(
                    "BIT %0d : MOSI=%b, MISO=%b, rx_shift=%b, bit_count=%0d",
                    i,
                    spi_mosi,
                    recv_data[i],
                    uut.rx_shift,
                    uut.bit_count
                );


                #10;


                //--------------------------------
                // SCLK Falling
                //--------------------------------
                spi_sclk = 1'b0;

                #50;

            end


            // 마지막 비트 내부 처리 완료
            #100;


            $display("--------------------------------");
            $display("Slave received : %b (0x%02h)", rx_data, rx_data);
            $display("Master received: %b (0x%02h)", recv_data, recv_data);
            $display("rx_valid       : %b", rx_valid);
            $display("--------------------------------");


            spi_cs_n = 1'b1;
            spi_mosi = 1'b0;

            #100;

        end

    endtask



    // rx_valid 발생 순간 별도 확인
    always @(posedge clk) begin
        if(rx_valid) begin
            $display("");
            $display("***** RX VALID *****");
            $display("RX DATA = %b = 0x%02h", rx_data, rx_data);
            $display("********************");
        end
    end



    initial begin

        clk      = 1'b0;
        reset    = 1'b1;

        spi_sclk = 1'b0;
        spi_cs_n = 1'b1;
        spi_mosi = 1'b0;

        tx_data  = 8'hA5;


        // reset
        #50;
        reset = 1'b0;

        // tx_shift preload
        #100;


        //--------------------------------
        // TEST 1
        //--------------------------------
        spi_transfer(8'h3C, received_miso);


        if(rx_data == 8'h3C)
            $display("MOSI -> Slave RX : PASS");
        else
            $display("MOSI -> Slave RX : FAIL");

        if(received_miso == 8'hA5)
            $display("MISO -> Master RX : PASS");
        else
            $display("MISO -> Master RX : FAIL");


        //--------------------------------
        // TEST 2
        //--------------------------------

        tx_data = 8'h5A;

        #100;

        spi_transfer(8'hC7, received_miso);


        if(rx_data == 8'hC7)
            $display("MOSI -> Slave RX : PASS");
        else
            $display("MOSI -> Slave RX : FAIL");

        if(received_miso == 8'h5A)
            $display("MISO -> Master RX : PASS");
        else
            $display("MISO -> Master RX : FAIL");


        #200;
        $finish;

    end

endmodule