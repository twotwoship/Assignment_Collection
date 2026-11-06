`timescale 1ns / 1ps

// CTRL[2] = START_EN, CTRL[1] = STOP_EN, CTRL[0] = RW
// STATUS[2:0] = {busy, done, nack_error}

module SENSOR_READER#(
    parameter READ_PERIOD_COUNT = 100_000_000
)(
    input clk, reset,
    input [7:0] rx_data,
    input [2:0] status,
    output reg [47:0] value,
    output reg request,
    output reg [2:0] ctrl,
    output reg [15:0] led,
    output reg [7:0] tx_data
);

parameter wait_period = 3'b000; parameter ptr_req = 3'b001;
parameter ptr_wait = 3'b010; parameter read_req = 3'b011;
parameter read_wait = 3'b100; parameter store = 3'b101;
parameter update = 3'b110; parameter error = 3'b111;

reg [2:0] r_state;
reg [31:0] r_period_count;
reg [2:0] r_index;
reg [7:0] r_rx;

reg [7:0] r_hum_int, r_hum_dec;
reg [7:0] r_temp_int, r_temp_dec;
reg [7:0] r_dist_h, r_dist_l;

always @(posedge clk or posedge reset) begin
    if(reset) begin
        r_state <= wait_period;
        r_period_count <= 0; r_index <= 0; r_rx <= 8'h00;

        r_hum_int <= 8'h00; r_hum_dec <= 8'h00;
        r_temp_int <= 8'h00; r_temp_dec <= 8'h00;
        r_dist_h <= 8'h00; r_dist_l <= 8'h00;

        value <= 48'h000000000000;
        request <= 1'b0; ctrl <= 3'b000;
        tx_data <= 8'h00; led <= 16'h0000;
    end else begin
        // 요청 상태에서만 한 클럭 동안 1
        request <= 1'b0;

        case(r_state)
        wait_period: begin
            if(r_period_count == READ_PERIOD_COUNT - 1) begin
                r_period_count <= 0;
                r_index <= 0;

                led[12] <= 1'b1;  // RUN
                led[13] <= 1'b0;  // NACK 초기화
                led[14] <= 1'b0;  // FAIL 초기화
                led[15] <= 1'b0;  // PASS 초기화

                r_state <= ptr_req;
            end else begin
                r_period_count <= r_period_count + 1'b1;
            end
        end

        ptr_req: begin
            if(!status[2]) begin
                tx_data <= 8'h00;  // 읽기 시작 포인터
                ctrl <= 3'b110;    // START=1, STOP=1, 쓰기
                request <= 1'b1;
                r_state <= ptr_wait;
            end
        end

        ptr_wait: begin
            if(status[1]) begin
                if(status[0]) begin
                    r_state <= error;
                end else begin
                    r_index <= 0;
                    r_state <= read_req;
                end
            end
        end

        read_req: begin
            if(!status[2]) begin
                tx_data <= 8'h00;  // 읽기에서는 미사용

                if(r_index == 0)
                    ctrl <= 3'b101;  // START 후 읽기·ACK
                else if(r_index == 5)
                    ctrl <= 3'b011;  // 마지막 읽기·NACK·STOP
                else
                    ctrl <= 3'b001;  // 이어서 읽기·ACK

                request <= 1'b1;
                r_state <= read_wait;
            end
        end

        read_wait: begin
            if(status[1]) begin
                if(status[0]) begin
                    r_state <= error;
                end else begin
                    r_rx <= rx_data;
                    r_state <= store;
                end
            end
        end

        store: begin
            case(r_index)
                0: r_hum_int <= r_rx;
                1: r_hum_dec <= r_rx;
                2: r_temp_int <= r_rx;
                3: r_temp_dec <= r_rx;
                4: r_dist_h <= r_rx;
                5: r_dist_l <= r_rx;
            endcase

            led[7:0] <= r_rx;  // 가장 최근에 받은 바이트

            if(r_index == 5) begin
                r_state <= update;
            end else begin
                r_index <= r_index + 1'b1;
                r_state <= read_req;
            end
        end

        update: begin
            // 정상 수신한 6바이트를 한꺼번에 출력
            value <= {r_hum_int, r_hum_dec,
                      r_temp_int, r_temp_dec,
                      r_dist_h, r_dist_l};

            led[12] <= 1'b0; led[13] <= 1'b0;
            led[14] <= 1'b0; led[15] <= 1'b1;

            r_period_count <= 0;
            r_state <= wait_period;
        end

        error: begin
            // value는 이전 정상 수신 값을 유지
            led[12] <= 1'b0; led[13] <= status[0];
            led[14] <= 1'b1; led[15] <= 1'b0;

            r_period_count <= 0;
            r_state <= wait_period;
        end

        default: begin
            r_state <= wait_period;
            r_period_count <= 0; r_index <= 0;

            led[12] <= 1'b0;
            led[14] <= 1'b1;
            led[15] <= 1'b0;
        end
        endcase
    end
end

endmodule