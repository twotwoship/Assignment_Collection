`timescale 1ns / 1ps

module TOP_MASTER(input clk, reset, inout sda_ja1, scl_ja2, sda_adc1, scl_adc2, output [15:0]led);

wire w_sensor_request; wire w_oled_request; wire [7:0]w_sensor_rx; wire [7:0]w_oled_rx; wire [2:0]w_sensor_status;
wire [2:0]sensor_ctrl; wire [2:0]oled_ctrl; wire [7:0]w_sensor_tx; wire [7:0]w_oled_tx; wire [2:0]w_oled_status;
I2C_MASTER #(.slave_addr(7'h42))SENSOR_I2C_MASTER(.clk(clk), .reset(reset), .sda(sda_ja1), .scl(scl_ja2), .request(w_sensor_request), 
.ctrl(sensor_ctrl), .tx_data(w_sensor_tx), .rx_data(w_sensor_rx), .status(w_sensor_status));

I2C_MASTER #(.slave_addr(7'h3C))OLED_I2C_MASTER(.clk(clk), .reset(reset), .sda(sda_adc1), .scl(scl_adc2), .request(w_oled_request), 
.ctrl(oled_ctrl), .tx_data(w_oled_tx), .rx_data(w_oled_rx), .status(w_oled_status));

wire [47:0]w_value; wire [15:0]w_led;
SENSOR_READER u_SENSOR_READER(.clk(clk), .reset(reset), .rx_data(w_sensor_rx), .status(w_sensor_status), 
.value(w_value), .request(w_sensor_request), .ctrl(sensor_ctrl), .led(w_led), .tx_data(w_sensor_tx));

OLED_CONTOLLER u_OLED_CONTOLLER(.clk(clk), .reset(reset), .value(w_value), .status(w_oled_status), 
 .rx_data(w_oled_rx), .request(w_oled_request), .ctrl(oled_ctrl), .tx_data(w_oled_tx));

LED_CONTOLLER u_LED_CONTOLLER(.clk(clk), .reset(reset), .in_led(w_led), .out_led(led));

endmodule
