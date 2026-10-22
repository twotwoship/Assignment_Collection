`timescale 1ns / 1ps

// moor fsm: 입력 x에서 101 시퀀스(중첩허용)
//           출력 y ( S3상태로 왔을때 y가 1로 된다)
// 변경: btn_press(입력 유효 펄스)이 1인 클럭에서만 상태가 전이된다.
//       (버튼을 눌렀다 뗄 때마다 비트 1개가 입력되도록 하기 위함)
module moor_fsm_101(
    input clk,
    input reset,
    input btn_press,
    input x,
    output y
    );

    // 상태 인코딩 (S0:대기 S1:1검출 S2:10검출 S3:101검출)
    localparam [1:0] S0 = 2'b00,    // IDLE
                     S1 = 2'b01,     // 1검출
                     S2 = 2'b10,     // 10검출
                     S3 = 2'b11;    // 101검출
    
    reg [1:0] curr_state, next_state;

    // 1. curr state 상태 register (순차회로)
    always @(posedge clk, posedge reset) begin
        if (reset) curr_state <= S0;
        else curr_state <= next_state;   
    end 

    // 2. 다음 상태 로직 (조합회로)
    always @(*) begin
        next_state = curr_state;   // !btn_press면 현재 상태 유지 
        if(btn_press) begin    // btnU나 btnD 둘중 1개를 누른 시점에만 상태 천이 
            case (curr_state)
            S0: next_state = x ? S1 : S0;
            S1: next_state = x ? S1 : S2;
            S2: next_state = x ? S3 : S0;
            S3: next_state = x ? S1 : S2;    // 중첩 허용
            default:  next_state = S0;   // 레치 방지 
            endcase
        end 
    end

    // 3. 출력 로직 : 현재 상태로만 출력 (moore machine)
    assign y = (curr_state == S3);
endmodule
