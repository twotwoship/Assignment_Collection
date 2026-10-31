# ATmega128A Washing Machine Controller

A washing machine controller with wash, rinse, and spin modes. Buttons and UART commands control the cycle.

Presentation file: `가. _LeeYB_Washing_machine_ppt.pptx`

**Technologies:** ATmega128A · C · FSM · Timer Interrupts · PWM · UART

## Demo

[Watch the washing machine demo](https://www.youtube.com/shorts/kGAorJIMjIk)

## What it does

- State transitions between wash, rinse, and spin cycles
- Button controls for setting the operating time and starting or stopping the machine
- PWM control of DC motor speed and direction
- Start and stop commands over UART
- Remaining time shown on a seven-segment display (FND)
- Timekeeping through timer interrupts

## Files

| File | Purpose |
|---|---|
| [main.c](./main.c) | Initialization and timer interrupts |
| [pwm.c](./pwm.c) | PWM and motor control |
| [uart0.c](./uart0.c) | UART transmission, reception, and command handling |
| [button.c](./button.c) | Button input handling |
| [FND.c](./FND.c) | Seven-segment display output |
| [led.c](./led.c) | LED control |

## Things to improve

- The loops inside state functions make it hard to handle events shared across modes.
- Display, input, and motor code are too closely tied to the state logic.
- Invalid commands and error cases need more work.
