`timescale 1ns / 1ps

module tb_moor_fsm_101();
    reg clk; reg reset; reg x; reg x1; wire y; reg r_error = 1'b0;
    moor_fsm_101 u_moor_fsm_101(.clk(clk), .reset(reset), .x(x), .x1(x1), .y(y));

always #5 clk = ~clk;

// Checking whether the output y changes before the rising edge
// checking whether the output y expected after the rising edge
task send_data(input a, input a1, input expect_val);
begin
    x = a; x1 = a1; #1; // gate operation
    @(posedge clk); #1;
    if(y !== expect_val) begin 
        // If y is any value other than the expected value, a logic error occurs in the DUT.
        r_error = 1'b1; // error detect flag
        $display("FAIL : t%0t, x=%b, x1=%b, y = %d, expect_val = %b", $time, a, a1, y, expect_val);
    end
end
endtask

initial begin
    clk = 0;    reset = 0;    x = 0; x1 = 0;
    #100; reset = 1;
    #100; reset = 0;
    //10101 101
    send_data(1,0,0); send_data(0,1,0); send_data(1,0,0); send_data(0,1,0); send_data(1,0,0);
    send_data(1,0,0); send_data(0,1,0); send_data(1,0,0); send_data(0,1,0);
    if( r_error == 0) $display("siuuuuu moor fsm_101");
    else $display(" pass siuuuuu moor fsm_101");
    $finish;
end
endmodule
