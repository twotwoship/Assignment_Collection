# ATmega128A Digital Clock and Calculator

> A DS1307 RTC clock and a stack-based expression calculator

## 1. Project Overview

| Item | Description |
|---|---|
| Objective | Build a clock and calculator using an RTC, LCD, and keypad |
| MCU | ATmega128A |
| Language | C |
| Technologies | I²C, FSM, Timer Interrupts, Stacks |
| Inputs | 4×4 Matrix Keypad, Push Buttons |
| Output | LCD1602 |
| Clock Module | DS1307 RTC |

## 2. Presentation and Demo

- **Presentation slides:** Included in this directory
- [Presentation video](https://www.youtube.com/watch?v=l9xRuBvnAL4)
- [Demo video](https://www.youtube.com/watch?v=GpRSiZHryco)

## 3. Key Features

| Area | Implementation |
|---|---|
| Clock | Read the RTC date and time and display them on the LCD |
| Time Settings | Edit the date and time using buttons and save them to the RTC |
| Calculator | Enter numbers and operators through the keypad and display results |
| Expression Processing | Convert infix expressions to postfix notation |
| Evaluation | Use stacks for arithmetic, parentheses, and operator precedence |
| State Management | Switch between clock, settings, and calculator modes |
| Input Handling | Generate button events and keypad scan flags through timer interrupts |
| Data Conversion | Convert RTC data between BCD and decimal |

## 4. Operating Modes

| Mode | Behavior |
|---|---|
| Clock | Display the date and time and refresh the screen periodically |
| Settings | Select a field, change its value, and update the RTC |
| Calculator | Enter and edit expressions and display the calculated result |

**Clock workflow**

Read RTC data → Convert BCD → Assemble the date and time → Update the LCD

**Calculator workflow**

Read keypad input → Store the expression → Convert to postfix → Evaluate with stacks → Update the LCD

## 5. Source Files

| File | Purpose |
|---|---|
| [main.c](./main.c) | Peripheral initialization and main loop |
| [menu.c](./menu.c) | Mode switching and state function dispatch |
| [ds1307.c](./ds1307.c) | I²C communication, RTC reads and settings, and BCD conversion |
| [text_lcd.c](./text_lcd.c) | LCD initialization, commands, and character output |
| [keypad.c](./keypad.c) | Keypad input handling |
| [keypad_cal_lcd.c](./keypad_cal_lcd.c) | Calculator input and LCD integration |
| [cal.c](./cal.c) | Expression conversion, stacks, and evaluation |
| [button.c](./button.c) | Button input and debouncing |
| [timer0.c](./timer0.c) | Timer interrupts and input event flags |
| [uart0.c](./uart0.c) | UART communication |

## 6. Design Decisions

- **State management:** Dispatch mode handlers through an array of function pointers.
- **Input events:** Set flags in interrupts and check them in mode handlers.
- **Display refresh:** Use timer counters for periodic updates.
- **RTC communication:** Handle I²C START, STOP, transmission, reception, and ACK/NACK.
- **Expression evaluation:** Keep operator and operand stacks separate.
