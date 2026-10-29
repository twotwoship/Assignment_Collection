`timescale 1ns / 1ps
module fsm_7(input clk, input reset, input btn_press, input x, output [15:0]led);
localparam [6:0]S0 = 7'b0000000; localparam [6:0]S1 = 7'b0000001; localparam [6:0]S2 = 7'b0000010;
localparam [6:0]S3 = 7'b0000101; localparam [6:0]S4 = 7'b0001010; localparam [6:0]S5 = 7'b0010101;
localparam [6:0]S6 = 7'b0101011; localparam [6:0]S7 = 7'b1010111;
reg[6:0] curr_state, next_state;
always@(posedge clk, posedge reset)begin
    if(reset) curr_state <= 0; else curr_state<= next_state;
end

always@(*) begin
    next_state = curr_state;
    if(btn_press) begin
       if(x) begin // up 눌럿을때 1
        case(curr_state)
            S0: next_state = S1; S1: next_state = S1; 
            S2: next_state = S3; S3: next_state = S1; 
            S4: next_state = S5; S5: next_state = S6; 
            S6: next_state = S7; S7: next_state = S1; 
            default: next_state = S0;
        endcase
    end else begin // down 눌럿을때 0
        case(curr_state)
            S0: next_state = S0; S1: next_state = S2; 
            S2: next_state = S0; S3: next_state = S4; 
            S4: next_state = S0; S5: next_state = S4; 
            S6: next_state = S2; S7: next_state = S2; 
            default: next_state = S0;
        endcase
    end 
    end
end
assign led[6:0] = curr_state; assign led[14:7] = 8'b0;
assign led[15] = (curr_state == S7);
endmodule
