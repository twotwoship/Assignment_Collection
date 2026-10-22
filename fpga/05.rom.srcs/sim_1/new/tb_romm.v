`timescale 1ns / 1ps

module tb_romm();
    reg [3:0]t_addr; reg t_oe; wire [7:0]t_dataout;

    romm u_room(t_addr, t_oe, t_dataout);

    initial begin
        #10; t_oe = 0; t_addr = 4'b0000;
        #10; t_oe = 0; t_addr = 4'b0001;
        #10; t_oe = 1; t_addr = 4'b0001;
        #10; t_oe = 1; t_addr = 4'b0010;
        #10; $finish;    
    end
    
endmodule
