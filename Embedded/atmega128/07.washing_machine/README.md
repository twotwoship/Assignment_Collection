# ATmega128A Washing Machine Controller

An MCU project that controls the wash, rinse, and spin cycles through state transitions, with button and UART command inputs.

Presentation file: `가. _LeeYB_Washing_machine_ppt.pptx`

**Technologies:** ATmega128A · C · FSM · Timer Interrupts · PWM · UART

## Demo

[Watch the washing machine demo](https://www.youtube.com/shorts/kGAorJIMjIk)

## Key Features

- State transitions between wash, rinse, and spin cycles
- Button controls for setting the operating time and starting or stopping the machine
- PWM control of DC motor speed and direction
- Start and stop commands over UART
- Remaining time shown on a seven-segment display (FND)
- Timekeeping through timer interrupts

## Source Files

| File | Purpose |
|---|---|
| [main.c](./main.c) | Initialization and timer interrupts |
| [pwm.c](./pwm.c) | PWM and motor control |
| [uart0.c](./uart0.c) | UART transmission, reception, and command handling |
| [button.c](./button.c) | Button input handling |
| [FND.c](./FND.c) | Seven-segment display output |
| [led.c](./led.c) | LED control |

## Known Limitations

- Loops inside state functions make shared event handling difficult to extend.
- State control is tightly coupled to display, input, and motor handling.
- Invalid commands and exceptional conditions need more robust handling.
