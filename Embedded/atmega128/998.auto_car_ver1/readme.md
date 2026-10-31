# ATmega128A Obstacle-Avoidance and Bluetooth-Controlled Car

> Ultrasonic obstacle avoidance and manual Bluetooth control
>
> Project report included

## 1. Project Overview

| Item | Description |
|---|---|
| Objective | Control a car by integrating sensors, communication, and motors |
| MCU | ATmega128A |
| Language | C |
| Technologies | FSM, UART, PWM, Timers, External Interrupts |
| Inputs | Three Ultrasonic Sensors, Bluetooth, Buttons |
| Outputs | DC Motors, Seven-Segment Display (FND), LEDs |

## 2. Demo

- [Watch the driving demo](https://www.youtube.com/shorts/NwpaM5rJNO4)

## 3. Key Features

| Area | Implementation |
|---|---|
| Manual Driving | Move forward, reverse, turn left or right, and stop using Bluetooth commands |
| Autonomous Driving | Avoid obstacles by comparing left, center, and right distance readings |
| Motor Control | Control speed and direction with PWM |
| State Management | Manual, autonomous, and driving-record review modes |
| Communication | Receive commands through UART receive interrupts |
| Display | Show driving information on the seven-segment display |

## 4. Non-Blocking Processing Improvements

| Area | Previous Approach | Revised Approach |
|---|---|---|
| Trigger Generation | Wait using `_delay_us()` | Check elapsed time using a timer counter |
| Processing Structure | Set HIGH, wait, then set LOW in sequence | Separate setting HIGH, checking time, and switching LOW |
| Echo Detection | External interrupts | Use rising- and falling-edge flags |
| Scope | Existing sensor processing | Apply the revised trigger handling to the left sensor first |

**Processing sequence**

1. Set the trigger HIGH and record the start time.
2. Check the elapsed time and set the trigger LOW.
3. Detect the rising and falling echo edges through external interrupts.
4. Check completion flags and prepare for the next measurement.

## 5. Current Status

- Non-blocking trigger code has been written for the left sensor.
- The corresponding processing function runs in the main loop.
- The driving FSM call is commented out.
- Integration with the full driving controller and operational verification are still required.

## 6. Source Files

| File | Purpose |
|---|---|
| [main.c](./main.c) | Initialization and experimental loop |
| [menu.c](./menu.c) | Driving modes and state management |
| [motor.c](./motor.c) | Motor drive and speed control |
| [ultrasonic.c](./ultrasonic.c) | Ultrasonic trigger and measurement handling |
| [interrupt.c](./interrupt.c) | Timer and external interrupts |
| [uart1.c](./uart1.c) | Bluetooth communication |
| [uart0.c](./uart0.c) | PC UART communication |
| [fnd.c](./fnd.c) | Driving information display |
| [button.c](./button.c) | Button input handling |
