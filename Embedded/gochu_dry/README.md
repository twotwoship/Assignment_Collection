# STM32 Chili Pepper Dryer | Temperature Measurement and Heater Control

Integrates a temperature sensor, buttons, a relay, a seven-segment display (FND), and an OLED.

## Key Features

- Request DS18B20 temperature conversions, check completion, and read measurements
- Increase, decrease, and confirm the target temperature using buttons, wrapping between 0 and 99
- Control the heater relay according to the start switch and measured temperature
- Display the temperature on the FND through TIM3 interrupts
- Show the target temperature, relay state, and startup animation on an I²C OLED

## Workflow

Initialize → Show the OLED startup screen → Handle button events → Request and check temperature conversion → Check the start switch → Control the heater.

The FND is updated in the TIM3 interrupt handler.

## Hardware Connections

| Function | Pin |
|---|---|
| DS18B20 Data | PA3 |
| Increase / Confirm / Decrease Temperature | PB0 / PB1 / PB2 |
| Start Switch | PB12 |
| Heater Relay | PB5 |
| LED 1 / 2 | PB6 / PB7 |
| FND RCLK / DIO / SCLK | PB13 / PB14 / PB15 |
| OLED I²C2 SCL / SDA | PB10 / PB11 |

- [OJ Tube reference video](https://www.youtube.com/watch?v=YAkAKx72prs): A reference tutorial, not a demo of this project.
