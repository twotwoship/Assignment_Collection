# Yangbae Lee

Embedded projects and study notes. Most of the work here covers MCU control, Linux drivers, and communication between devices.

KCCI Seoul Technical Education Center · AI System Semiconductor Software Developer Program, Cohort 2  
[GitHub](https://github.com/twotwoship) · [YouTube](https://www.youtube.com/@twotwoship)

## Linux projects

### Board Guard — PCB inspection and sorting

I worked on PCB detection and inspection states, PASS/FAIL logic, and communication between the Jetson and STM32. I also handled the state machine for the stepper and servo motors used to sort rejected boards.

C · Python · STM32F411RE · Jetson Orin Nano  
[Code](https://github.com/clap-min99/board_guard) · [Demo](https://www.youtube.com/shorts/YxTkjOZ_dDI) · [Report](https://github.com/twotwoship/Assignment_Collection/tree/main/AI_Deep_Learning/board_guard)

### Smart Fan — Linux device drivers

I wrote the LCD and rotary encoder drivers, using interrupts, FIFOs, and `read()`, `write()`, and `poll()` for device I/O.

C · Linux Kernel · GPIO · I²C · Jetson Orin Nano  
[Code](https://github.com/twotwoship/Linux_fan) · [Demo](https://www.youtube.com/shorts/Q2UGQjDw2Ow)

### YAKSLOT — medication dispenser

I led the team and handled the system design and integration. My work included the Raspberry Pi software, Flask and SQLite, the LCD interface, voice prompts, and UART communication.

Python · Raspberry Pi 4 · Flask · SQLite · UART  
[Code](https://github.com/twotwoship/2026ESWContest_free_YAKSLOT) · [Demo](https://www.youtube.com/watch?v=20yo6nAbsLM)

## MCU projects

- **[STM32 Home IoT wall panel](https://github.com/twotwoship/home_plus)**  
  My part was reading the temperature/humidity, ultrasonic, and light sensors, and controlling the motors, LEDs, and buzzer.  
  `STM32` · `C` · `ADC` · `Timer/PWM` · [Demo](https://youtube.com/shorts/tmq6LoPIN-Y?si=yGTwd4x-6AmtZ_au)

- **[ATmega128A clock and calculator](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/15.RTC_LCD_calculator_)**  
  DS1307 over I²C, LCD and keypad input, and a stack-based calculator.  
  `ATmega128A` · `C` · `I²C` · `FSM` · [Demo](https://www.youtube.com/watch?v=GpRSiZHryco) · [Presentation](https://www.youtube.com/watch?v=l9xRuBvnAL4)

- **[ATmega128A Bluetooth car](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/998.auto_car_ver1)**  
  Manual Bluetooth control and ultrasonic obstacle avoidance, with PWM motor control.  
  `ATmega128A` · `C` · `UART/Bluetooth` · `FSM` · [Demo](https://www.youtube.com/shorts/NwpaM5rJNO4)

- **[ATmega128A washing machine](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/07.washing_machine)**  
  Wash, rinse, and spin states, timer interrupts, PWM motor control, and UART commands.  
  `ATmega128A` · `C` · `Timer/PWM` · `FSM` · [Demo](https://www.youtube.com/shorts/kGAorJIMjIk)

- **[STM32 chili pepper dryer](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/gochu_dry)**  
  README still in progress.  
  `STM32` · `C` · `Timer/PWM` · `FSM`

## More projects and exercises

<details>
<summary>FPGA and on-device AI</summary>

- **[FPGA clock and stopwatch](https://github.com/twotwoship/fpga_clock)**  
  FSM-based RTL and simulation.  
  `Verilog HDL` · `Vivado` · `FPGA`

- **[FPGA microwave](https://github.com/twotwoship/fpga_microwave)**  
  FSM-based RTL and simulation.  
  `Verilog HDL` · `Vivado` · `FPGA` · [Demo](https://www.youtube.com/watch?v=djOuPST5QnM)

- **[Two-player rock–paper–scissors on Jetson](https://github.com/twotwoship/Assignment_Collection/tree/main/AI_Deep_Learning/%EA%B3%BC%EC%A0%9C%EB%AC%BC(%EA%B0%80%EC%9C%84%EB%B0%94%EC%9C%84%EB%B3%B4))**  
  Camera input, TensorRT inference, game logic, and Flask/MJPEG streaming.  
  `Python` · `Jetson Orin Nano` · `TensorRT` · `Flask`  
  I have the code and models locally. The setup and test results still need to be written up.

</details>

<details>
<summary>MCU peripherals and communication</summary>

- **[STM32 motor control](https://github.com/twotwoship/stm_motor_260724)**  
  DC motor speed and direction control using buttons and UART.  
  `STM32F411RE` · `C` · `UART` · `PWM`

</details>

<details>
<summary>Practice folders</summary>

- [AVR](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128): Peripherals, interrupts, and communication
- [STM32](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/stm32): Cortex-M exercises
- [FPGA](https://github.com/twotwoship/Assignment_Collection/tree/main/fpga): Verilog exercises

</details>

## What I'm studying

### BeagleBone Black bootloader

The ARM startup code, stack/BSS initialization, watchdog disable, UART0 code, and MLO build are in place. Next are a boot test on the board, exception handling, GPIO, DDR initialization, and loading a second stage.

[Code and notes](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/bbb/bbb-myrtos-stage1)

### AES-XTS storage on Zynq

Still at the design and study stage. The goal is to connect a Linux driver to an FPGA encryption path using MMIO, DMA, and IRQs.

[Plan](https://github.com/twotwoship/Assignment_Collection/tree/main/fpga_base_secure_storage_controller_datapath_proto) · [Study materials](https://github.com/twotwoship/Assignment_Collection/tree/main/aes_xts_nand)

## Folders

| Folder | Contents |
|---|---|
| `DataStructure` | Stacks, queues, calculator, binary search |
| `Algorithms` | C algorithm exercises |
| `Embedded/atmega128` | AVR peripherals and applications |
| `Embedded/stm32` | STM32 / Cortex-M exercises |
| `Embedded/bbb` | BeagleBone Black boot and bare-metal notes |
| `fpga` | Verilog / Vivado exercises |
| `fpga_base_secure_storage_controller_datapath_proto` | Linux–FPGA AES-XTS storage plan |
| `aes_xts_nand` | AES-XTS study and verification notes |
| `tcg_storage_opal` | TCG Storage Opal study |
