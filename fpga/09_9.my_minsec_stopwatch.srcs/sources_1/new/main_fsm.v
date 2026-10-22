`timescale 1ns / 1ps
module main_fsm(input clk, reset, [2:0]btn, output [23:0]seg, output mode_sel, mode2_sel);
    /* 시계, 스톱워치, 정지, 리셋
    시계는 따로 스톱워치도 따로. 추가 구현사항 스탑모드에서 3초가 지나면 돌아가는 애니메이션이 나와야됨
    그러면 수정해야될 사항이 나가는 모드가 추가되어야 할까?'
    스탑모드일때 5초가 지나면 모드가 전환되게 하자 그러면 모드를 하나 추가해야되고 한칸 더 늘려야되네? 하 mode2_sel 이게 1이되면 아니매가 켜지고 0이면 정상스탑워치
     mode2_sel 이게 1이되면 아니매가 켜지고 0이면 정상스탑워치 1이 되는 조건은 스톱모드에서 5초가 지나면 1이되고 다시 스톱이 눌리면 0
     mode1이건 시계냐 스톱워치냐 구분
    */
    parameter CLOCK = 2'b00; parameter STOPWATCH = 2'b01; parameter STOP = 2'b10; parameter RESET = 2'b11;
    parameter NONE = 3'b000; parameter btnL = 3'b001; parameter btnC = 3'b010; parameter btnR = 3'b100;
    parameter NS_LIMIT = 100_000_0;

    reg[1:0] r_mode = CLOCK;  reg [2:0]r_prev_btn = 3'b000; reg r_mode_sel = 0; reg r_mode2_sel = 0;
    reg[$clog2(NS_LIMIT)-1:0] r_clock_10ns_counter;
    reg[$clog2(NS_LIMIT)-1:0] r_stopwatch_10ns_counter;
    reg[$clog2(1_000_000 * 100)-1 : 0] r_clock_10ms_counter; 
    reg[$clog2(10000)-1 : 0] r_stopwatch_10ms_counter;
    reg[$clog2(NS_LIMIT)-1:0] r_ani_10ns_counter;
    reg[$clog2(10000)-1 : 0] r_ani_10ms_counter;
    
    /* 지속이아닌 한번 눌린걸로 체크하는거임. 따아아아잇! 모드ㅊㅋ
    */
    always@(posedge clk or posedge reset) begin
        if(reset) begin r_mode <= CLOCK; r_prev_btn <= NONE; r_mode_sel <= 0; end
        else if((r_prev_btn != NONE) && (r_prev_btn != btn))begin // 이전 버튼이 아무것도 안 눌린상태가 아니고 이전버튼이 현재 버튼값과 다를때.
            case(r_mode)
                CLOCK:              if(r_prev_btn == btnL) begin r_mode <= STOPWATCH; r_mode_sel <= 1; end
                STOPWATCH: begin    if(r_prev_btn == btnR) r_mode <= STOP; 
                                    else if(r_prev_btn == btnC) begin r_mode <= RESET; r_mode_sel <= 1; end
                                    else if(r_prev_btn == btnL) begin r_mode <= CLOCK; r_mode_sel <= 0; end end
                STOP:               if(r_prev_btn == btnR) begin r_mode <= STOPWATCH; r_mode_sel <= 1; end
                RESET:              if(r_prev_btn == btnC) begin r_mode <= STOPWATCH; r_mode_sel <= 1; end
                default:            begin r_mode <= CLOCK; r_mode_sel <= 0; end
            endcase
        end
        r_prev_btn <= btn;
    end
    /* 타이마 돌려 후루꾸*/
    always@(posedge clk or posedge reset)begin
        if(reset) begin r_clock_10ns_counter<= 0; r_stopwatch_10ns_counter<= 0; r_clock_10ms_counter <= 0; r_stopwatch_10ms_counter <= 0; 
                        r_ani_10ns_counter <= 0; r_ani_10ms_counter <= 0; r_mode2_sel <= 0; end
        else begin
        // 애니매 도는거
            if(r_mode == STOP) begin
                if(r_ani_10ms_counter >= 500) r_mode2_sel <= 1;
                if(r_ani_10ns_counter == NS_LIMIT - 1) begin
                    r_ani_10ns_counter <= 0;
                    if(r_ani_10ms_counter >= 9999) r_ani_10ms_counter <= 501;
                    else r_ani_10ms_counter <= r_ani_10ms_counter + 1;
                end else r_ani_10ns_counter <= r_ani_10ns_counter + 1;
            end
        // 띵킹 스톱와치는 스톱와츠 들어갓을때만 돌아가고 시계는 계속 돌아야됨 알바노하고 그럼 시계는 밖에서 돌아 아닌가? 맞네
            if(r_clock_10ns_counter == NS_LIMIT - 1) begin //일단 시계는 상태와 무관하게 독립적으로 돈다. 빙빙돈다.
                r_clock_10ns_counter <= 0; 
                if(r_clock_10ms_counter >= 599_999) r_clock_10ms_counter <= 0; // 시계돌아잇
                else r_clock_10ms_counter <= r_clock_10ms_counter + 1;
            end else r_clock_10ns_counter <= r_clock_10ns_counter + 1; // 일단 더하고 생각해.

            if(r_mode == STOPWATCH) begin // 스탑와치들어가면 바로 그냥 스탑와치 돌아
                r_mode2_sel <=0; r_ani_10ms_counter <= 0;
                if(r_stopwatch_10ns_counter == NS_LIMIT - 1) begin
                    r_stopwatch_10ns_counter <= 0;
                    if(r_stopwatch_10ms_counter >= 9999) r_stopwatch_10ms_counter <= 0;
                    else r_stopwatch_10ms_counter <= r_stopwatch_10ms_counter + 1;
                end else r_stopwatch_10ns_counter <= r_stopwatch_10ns_counter + 1;
            end
            else if(r_mode == STOP) r_stopwatch_10ms_counter <= r_stopwatch_10ms_counter; // 아무것도 안해버리기 대신 타이머는 멈춰야되니까 상태넣어
            else if(r_mode == RESET) begin r_stopwatch_10ns_counter <= 0; r_stopwatch_10ms_counter <= 0; end // 초!기!화!
            else if(r_mode == CLOCK) r_stopwatch_10ms_counter <= 0;
        end
    end
    assign seg = (r_mode == CLOCK) ? r_clock_10ms_counter : r_stopwatch_10ms_counter;
    assign mode_sel = r_mode_sel;
    assign mode2_sel = r_mode2_sel;
endmodule
