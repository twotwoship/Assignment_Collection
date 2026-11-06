`timescale 1ns / 1ps

module tb_top;
    reg clk, reset;
    tri1 sda_ja1, scl_ja2, sda_adc1, scl_adc2;
    reg sensor_low, oled_low;
    reg [3:0] oled_bit_count;
    wire [15:0] led;

    assign sda_ja1  = sensor_low ? 1'b0 : 1'bz;
    assign sda_adc1 = oled_low   ? 1'b0 : 1'bz;

    TOP_MASTER dut (
        .clk(clk), .reset(reset), .led(led),
        .sda_ja1(sda_ja1), .scl_ja2(scl_ja2),
        .sda_adc1(sda_adc1), .scl_adc2(scl_adc2)
    );
    // 센서값 사이 100ms 간격: OLED 갱신 관찰용
    defparam dut.u_SENSOR_READER.READ_PERIOD_COUNT = 10_000_000;
    defparam dut.u_OLED_CONTOLLER.POWER_WAIT_COUNT  = 1_000;
    defparam dut.u_OLED_CONTOLLER.UPDATE_COUNT      = 1_000;

    initial clk = 0;
    always #5 clk = ~clk;

    // OLED: START마다 카운트 초기화, 9번째 클럭에서 ACK
    always @(negedge sda_adc1)
        if (scl_adc2 === 1'b1) oled_bit_count = 0;

    always @(posedge scl_adc2)
        if (!reset) begin
            if (oled_bit_count == 8) oled_bit_count = 0;
            else oled_bit_count = oled_bit_count + 1'b1;
        end
    always @(negedge scl_adc2) begin
        #1;
        oled_low = !reset && (oled_bit_count == 8);
    end
    // 센서 ACK: nack=0 정상 / nack=1 응답 거부
    task sensor_ack;
        input nack;
        begin
            @(negedge scl_ja2);
            #1; sensor_low = !nack;
            @(posedge scl_ja2);
            @(negedge scl_ja2);
            #1; sensor_low = 0;
        end
    endtask
    // 센서 데이터: MSB부터 전송, 9번째 클럭은 마스터 응답
    task sensor_byte;
        input [7:0] data;
        integer i;
        begin
            for (i = 7; i >= 0; i = i - 1) begin
                sensor_low = ~data[i];
                @(posedge scl_ja2);
                @(negedge scl_ja2);
                #1;
            end
            sensor_low = 0;
            @(posedge scl_ja2);
            @(negedge scl_ja2);
            #1;
        end
    endtask

    // 정해진 거래 순서: 주소 ACK, 포인터 ACK, 읽기 주소 ACK, 6바이트
    task sensor_reply;
        input [47:0] data;
        input nack;
        integer i;
        begin
            @(negedge sda_ja1); // 포인터 쓰기 START
            repeat (8) @(posedge scl_ja2);
            sensor_ack(nack);
            if (!nack) begin
                repeat (8) @(posedge scl_ja2); // 포인터 0x00
                sensor_ack(0);
                wait (sda_ja1 === 1'b1 && scl_ja2 === 1'b1); // STOP
                @(negedge sda_ja1); // 센서 읽기 START
                repeat (8) @(posedge scl_ja2);
                sensor_ack(0);
                for (i = 0; i < 6; i = i + 1)
                    sensor_byte(data[47-i*8 -: 8]);
            end
            wait (sda_ja1 === 1'b1 && scl_ja2 === 1'b1); // STOP
            #10_000; // done, value, LED 갱신 대기
        end
    endtask

    initial begin
        reset = 1; sensor_low = 0; oled_low = 0; oled_bit_count = 0;
        #100;
        @(negedge clk); reset = 0;
        // 1. 습도 60.5%, 온도 25.3C, 거리 30cm
        sensor_reply(48'h3C051903001E, 0);
        // 2. 습도 61.2%, 온도 26.4C, 거리 300cm
        sensor_reply(48'h3D021A04012C, 0);
        // 3. NACK: 이전 값 유지
        sensor_reply(48'h000000000000, 1);
        // 4. 정상 복구
        sensor_reply(48'h3C051903001E, 0);
        #100_000_000; // 복구값 OLED 전송 관찰
        $finish;
    end
    initial begin
        #1_000_000_000;
        $finish;
    end
endmodule
