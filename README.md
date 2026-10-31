# Yangbae Lee | Portfolio

A collection of projects covering MCU control, Linux device drivers, and inter-device communication.

KCCI Seoul Technical Education Center · AI System Semiconductor Software Developer Program, Cohort 2  
[GitHub](https://github.com/twotwoship) · [YouTube Demos](https://www.youtube.com/@twotwoship)

## Linux Projects

### Board Guard | Automated PCB Inspection and Reject Sorting

- **Contributions:** PCB presence and inspection state management, PASS/FAIL decision logic, Jetson–STM32 interface, and state-machine-based reject sorting with stepper and servo motors
- **Technologies:** C · Python · STM32F411RE · Jetson Orin Nano
- **Resources:** [Code and Details](https://github.com/clap-min99/board_guard) · [Demo](https://www.youtube.com/shorts/YxTkjOZ_dDI) · [Report](https://github.com/twotwoship/Assignment_Collection/tree/main/AI_Deep_Learning/board_guard)

### Smart Fan | Fan Control with Linux Device Drivers

- **Contributions:** LCD and rotary encoder drivers; device I/O using interrupts, FIFOs, and `read()`, `write()`, and `poll()`
- **Technologies:** C · Linux Kernel · GPIO · I²C · Jetson Orin Nano
- **Resources:** [Code and Details](https://github.com/twotwoship/Linux_fan) · [Demo](https://www.youtube.com/shorts/Q2UGQjDw2Ow)

### YAKSLOT | Automated Medication Assistance System

- **Contributions:** Team leadership, system architecture, and integration; Raspberry Pi software, Flask and SQLite, LCD UI, voice guidance, and UART communication
- **Technologies:** Python · Raspberry Pi 4 · Flask · SQLite · UART
- **Resources:** [Code and Details](https://github.com/twotwoship/2026ESWContest_free_YAKSLOT) · [Demo](https://www.youtube.com/watch?v=20yo6nAbsLM)

## MCU Projects

- **[STM32 Home IoT Wall Panel](https://github.com/twotwoship/home_plus)**  
  Handled temperature/humidity, ultrasonic, and light sensor inputs, and controlled DC and stepper motors, LEDs, and a buzzer.  
  `STM32` · `C` · `ADC` · `Timer/PWM` · [Demo](https://youtube.com/shorts/tmq6LoPIN-Y?si=yGTwd4x-6AmtZ_au)

- **[ATmega128A Digital Clock and Calculator](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/15.RTC_LCD_calculator_)**  
  DS1307 I²C communication, LCD and keypad integration, mode switching, and stack-based expression evaluation.  
  `ATmega128A` · `C` · `I²C` · `FSM` · [Demo](https://www.youtube.com/watch?v=GpRSiZHryco) · [Presentation](https://www.youtube.com/watch?v=l9xRuBvnAL4)

- **[ATmega128A Obstacle-Avoidance and Bluetooth-Controlled Car](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/998.auto_car_ver1)**  
  Ultrasonic distance measurement using external interrupts, PWM motor control, and manual/autonomous driving state management.  
  `ATmega128A` · `C` · `UART/Bluetooth` · `FSM` · [Demo](https://www.youtube.com/shorts/NwpaM5rJNO4)

- **[ATmega128A Washing Machine Controller](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/07.washing_machine)**  
  Wash/rinse/spin state transitions, timer-based timekeeping, PWM motor control, and UART command handling.  
  `ATmega128A` · `C` · `Timer/PWM` · `FSM` · [Demo](https://www.youtube.com/shorts/kGAorJIMjIk)

- **[STM32 Chili Pepper Dryer](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/gochu_dry)**  
  Description in progress. Demo link pending.  
  `STM32` · `C` · `Timer/PWM` · `FSM`

## Additional Projects and Exercises

<details>
<summary><strong>FPGA and On-Device AI Projects</strong></summary>

- **[FPGA Stopwatch / Clock](https://github.com/twotwoship/fpga_clock)**  
  FSM-based clock and stopwatch RTL design and simulation.  
  `Verilog HDL` · `Vivado` · `FPGA`

- **[FPGA Microwave](https://github.com/twotwoship/fpga_microwave)**  
  FSM-based microwave RTL design and simulation.  
  `Verilog HDL` · `Vivado` · `FPGA` · [Demo](https://www.youtube.com/watch?v=djOuPST5QnM)

- **[Jetson Two-Player Rock–Paper–Scissors Recognition and Web Streaming](https://github.com/twotwoship/Assignment_Collection/tree/main/AI_Deep_Learning/%EA%B3%BC%EC%A0%9C%EB%AC%BC(%EA%B0%80%EC%9C%84%EB%B0%94%EC%9C%84%EB%B3%B4))**  
  Implemented camera input, TensorRT inference, game state handling, and Flask/MJPEG web streaming.  
  `Python` · `Jetson Orin Nano` · `TensorRT` · `Flask`  
  Status: Code and models are available locally; the runtime environment and operational verification results still need to be documented.

</details>

<details>
<summary><strong>MCU Peripheral and Communication Exercises</strong></summary>

- **[STM32 Motor Control](https://github.com/twotwoship/stm_motor_260724)**  
  DC motor speed and direction control using button and UART inputs.  
  `STM32F411RE` · `C` · `UART` · `PWM`

</details>

<details>
<summary><strong>Fundamentals and Practice Code</strong></summary>

- [AVR](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128): Peripheral, interrupt, and communication exercises
- [STM32](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/stm32): Cortex-M MCU exercises
- [FPGA](https://github.com/twotwoship/Assignment_Collection/tree/main/fpga): Verilog RTL exercises

</details>

## Independent Study

### BeagleBone Black Stage-1 Bootloader

- **Current status:** ARM startup code, stack and BSS initialization, watchdog disable, UART0 control code, and MLO build artifacts are available.
- **Next steps:** Verify booting on hardware, extend exception handling and GPIO support, initialize DDR, and load a second stage.
- **Resources:** [Code and Scope](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/bbb/bbb-myrtos-stage1)

### Zynq Linux–FPGA AES-XTS Secure Storage

- **Current status:** System architecture design and preliminary study of AES-XTS and verification methods.
- **Development goal:** Integrate Linux driver MMIO, DMA, and IRQ handling with the FPGA encryption data path.
- **Resources:** [Project Design](https://github.com/twotwoship/Assignment_Collection/tree/main/fpga_base_secure_storage_controller_datapath_proto) · [AES-XTS Study Materials](https://github.com/twotwoship/Assignment_Collection/tree/main/aes_xts_nand)

## Directory Structure

| Category | Description |
|---|---|
| `DataStructure` | Stack, Queue, Calculator, Binary Search |
| `Algorithms` | C algorithm practice |
| `Embedded/atmega128` | AVR peripheral and application practice |
| `Embedded/stm32` | STM32 / Cortex-M practice |
| `Embedded/bbb` | BeagleBone Black boot / bare-metal study |
| `fpga` | Verilog / Vivado FPGA RTL practice |
| `fpga_base_secure_storage_controller_datapath_proto` | Linux-FPGA AES-XTS Secure Storage prototype |
| `aes_xts_nand` | AES-XTS study and verification workbook |
| `tcg_storage_opal` | TCG Storage Opal protocol study |

---
