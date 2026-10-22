`timescale 1ns / 1ps

// moor fsm input x, 101 sequence(allow_double)
//          output y, (when S3 state, y is 1)
module moor_fsm_101(input clk, input reset, input x, input x1, output y);
    // state incoding
    localparam [1:0]S0 = 2'b00; /// idle
    localparam [1:0]S1 = 2'b01; // 1 sel
    localparam [1:0]S2 = 2'b10; // 10 sel
    localparam [1:0]S3 = 2'b11; // 101 sel

    reg[1:0] curr_state, next_state;
    // 자 띵킹 input x가 1이면 1인거고 input x1이 1이면 0인거임 둘다 1이면? 몰?루 11일때랑 00일때는 현상 유지하자.
    reg r_x;
    wire [1:0]w_input = {x, x1};

    //1. curr state reg logic
    always @(posedge clk, posedge reset) begin
        if(reset) curr_state <= S0;
        else curr_state <= next_state;
    end
    
    //2. next state reg logic
    always @(*) begin
        next_state = curr_state;
        r_x = 0;
        if(w_input == 2'b00) begin
        end
        else if(w_input == 2'b10) begin // 10이 1이고 01이 0임   그리고 101 되야 하나임.
            r_x = 1;
           case(curr_state) 
            S0: next_state = r_x ? S1 : S0;
            S1: next_state = r_x ? S1 : S2;
            S2: next_state = r_x ? S3 : S0;
            S3: next_state = r_x ? S1 : S2;   // allowed double 
            default: next_state = S0;       // retch pro
        endcase 
        end
        else if(w_input == 2'b01) begin
            r_x = 0;
            case(curr_state) 
            S0: next_state = r_x ? S1 : S0;
            S1: next_state = r_x ? S1 : S2;
            S2: next_state = r_x ? S3 : S0;
            S3: next_state = r_x ? S1 : S2;   // allowed double 
            default: next_state = S0;       // retch pro
        endcase 
        end
        else if(w_input == 2'b11) begin
        end
    end

    //3. Output Logic: Output based only on the current state
    assign y = (curr_state == S3);
endmodule
