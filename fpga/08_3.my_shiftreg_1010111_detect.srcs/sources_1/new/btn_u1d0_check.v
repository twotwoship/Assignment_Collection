`timescale 1ns / 1ps

module btn_U1D0_check(
    input clk,
    input reset,   // 비동기 reset, active-high
    input clean_btnU,
    input clean_btnD,
    output btn_press,  
    output x    // btnU: 1, btnD: 0
    );

    reg r_prev_u, r_prev_d;     // 버튼 release(falling edge) 검출 -> 1클럭 펄스
    wire w_release_u, w_release_d;

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            r_prev_u <= 1'b0;
            r_prev_d <= 1'b0;
        end else begin
            r_prev_u <= clean_btnU;
            r_prev_d <= clean_btnD;
        end
    end

    assign w_release_u = r_prev_u & ~clean_btnU;   // btnU 뗀 순간
    assign w_release_d = r_prev_d & ~clean_btnD;   // btnD 뗀 순간

    assign btn_press = w_release_u | w_release_d;   // btnU 나 btnD 둘중 1개를 눌렀다 뗴면 
    assign x  = (w_release_u == 1'b1);                    // U: 1, D: 0

endmodule
