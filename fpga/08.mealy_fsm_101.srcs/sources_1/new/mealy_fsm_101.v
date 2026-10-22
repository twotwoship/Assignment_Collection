`timescale 1ns / 1ps

module mealy_fsm_101(input clk, input reset, input x, input x1, output reg y);

localparam [1:0]S0 = 2'b00;
localparam [1:0]S1 = 2'b01;
localparam [1:0]S2 = 2'b10;

reg [1:0] r_curr_state, r_next_state;
reg r_x;
wire [1:0]w_input = {x, x1};

// (1) curr_state reg(same moore)
always@(posedge clk or posedge reset) begin
    if(reset) r_curr_state <= S0;
    else r_curr_state = r_next_state;
end

// (2) next state + output (refer input)
always @(*) begin
    r_next_state = r_curr_state;
    y = 1'b0;
    r_x = 0;
    if(w_input == 2'b00) begin
    end
    else if(w_input == 2'b10) begin // 1
        r_x = 1;
        case(r_curr_state)
            S0: r_next_state = S1;
            S1: r_next_state = S1;
            S2: if(r_x) begin
            y = 1'b1;
            r_next_state = S1;
            end else r_next_state = S0;
            default: r_next_state = S0;
        endcase
    end
    else if(w_input == 2'b01) begin // 0
        r_x = 0;
        case(r_curr_state)
            S0: r_next_state = S0;
            S1: r_next_state = S2;
            S2: if(r_x) begin
            y = 1'b1;
            r_next_state = S1;
            end else r_next_state = S0;
            default: r_next_state = S0;
        endcase
    end
    else if(w_input == 2'b11) begin
    end
    
end
endmodule
