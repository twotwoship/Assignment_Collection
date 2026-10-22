`timescale 1ns / 1ps

module romm(input [3:0]addr, input oe, output reg [7:0]dataout );
    always @(*) begin
        case (oe)
            1'b1 : if(addr == 4'b0000) begin
                    dataout = 'h23;                   
                    end
                    else if(addr == 4'b0001) begin
                    dataout = 'hf3;                   
                    end
                    else if(addr == 4'b0010) begin
                    dataout = 'ha7;                 
                    end
                    else if(addr == 4'b0011) begin
                    dataout = 'h04;                
                    end
                    else if(addr == 4'b0100) begin
                    dataout = 'h06;                
                    end
                    else if(addr == 4'b0101) begin
                    dataout = 'h08;                
                    end
                    else if(addr == 4'b0110) begin
                    dataout = 'h10;
                    end
                    else if(addr == 4'b0111) begin
                    dataout = 'h12;                
                    end
                    else if(addr == 4'b1000) begin
                    dataout = 'h18;                
                    end
                    else if(addr == 4'b1001) begin
                    dataout = 'h16;                
                    end
                    else if(addr == 4'b1010) begin
                    dataout = 'h24;                
                    end
                    else if(addr == 4'b1011) begin
                    dataout = 'h26;                
                    end
                    else if(addr == 4'b1100) begin
                    dataout = 'h22;                
                    end
                    else if(addr == 4'b1101) begin
                    dataout = 'h28;                
                    end
                    else if(addr == 4'b1110) begin
                    dataout = 'h30;                
                    end
                    else if(addr == 4'b1111) begin
                    dataout = 'hd5;                
                    end
            1'b0 : dataout[7:0] = 8'hz;
            default : dataout = 8'b00000000;
        endcase
    end
endmodule
