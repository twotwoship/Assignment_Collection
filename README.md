# Yangbae Lee

**Embedded Linux · MCU Firmware · Device Drivers**

I write firmware and Linux drivers for embedded devices.  
My projects cover sensor input, motor control, and communication between boards.

[GitHub](https://github.com/twotwoship) · [YouTube Demos](https://www.youtube.com/@twotwoship)

---

### 🧭 Technical Focus

```text
Embedded Systems
│
├── Linux
│   ├── Device drivers: GPIO, I²C, interrupts
│   └── Device I/O: FIFO, read(), write(), poll()
│
├── MCU Firmware
│   ├── STM32 / ATmega128A
│   ├── Timers, PWM, ADC, and external interrupts
│   └── UART communication and state machines
│
└── FPGA & Low-Level Study
    ├── Verilog RTL design and simulation
    ├── BeagleBone Black startup and bootloader
    └── Zynq Linux–FPGA integration and AES-XTS
```

---

### 🔧 Selected Projects

#### Board Guard · PCB Inspection & Sorting

- Managed PCB detection, inspection states, and PASS/FAIL decisions.
- Handled Jetson–STM32 communication and the state machine for stepper/servo sorting.

`C` `Python` `STM32F411RE` `Jetson Orin Nano`

[Code](https://github.com/clap-min99/board_guard) · [Demo](https://www.youtube.com/shorts/YxTkjOZ_dDI) · [Report](https://github.com/twotwoship/Assignment_Collection/tree/main/AI_Deep_Learning/board_guard)

#### Smart Fan · Linux Device Drivers

- Wrote the LCD and rotary encoder drivers.
- Used interrupts, FIFOs, and `read()`, `write()`, and `poll()` for device I/O.

`C` `Linux Kernel` `GPIO` `I²C` `Jetson Orin Nano`

[Code](https://github.com/twotwoship/Linux_fan) · [Demo](https://www.youtube.com/shorts/Q2UGQjDw2Ow)

#### YAKSLOT · Medication Assistance System

- Led the team and handled system design and integration.
- Built the Raspberry Pi software, web/database functions, LCD interface, voice prompts, and UART communication.

`Python` `Raspberry Pi 4` `Flask` `SQLite` `UART`

[Code](https://github.com/twotwoship/2026ESWContest_free_YAKSLOT) · [Demo](https://www.youtube.com/watch?v=20yo6nAbsLM)

---

### ⚙️ MCU Projects

| Project | My Work | Video |
|---|---|---|
| [STM32 Home IoT](https://github.com/twotwoship/home_plus) | Sensor inputs, DC/stepper motors, LEDs, and buzzer | [Demo](https://youtube.com/shorts/tmq6LoPIN-Y?si=yGTwd4x-6AmtZ_au) |
| [ATmega128A Clock & Calculator](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/15.RTC_LCD_calculator_) | DS1307 I²C, LCD/keypad, and stack-based calculation | [Demo](https://www.youtube.com/watch?v=GpRSiZHryco) · [Presentation](https://www.youtube.com/watch?v=l9xRuBvnAL4) |
| [ATmega128A Bluetooth Car](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/998.auto_car_ver1) | Ultrasonic sensing, Bluetooth commands, and PWM motor control | [Demo](https://www.youtube.com/shorts/NwpaM5rJNO4) |
| [ATmega128A Washing Machine](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/07.washing_machine) | Wash/rinse/spin states, timers, and UART commands | [Demo](https://www.youtube.com/shorts/kGAorJIMjIk) |
| [STM32 Chili Pepper Dryer](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/gochu_dry) | Temperature sensing, heater relay, FND, and OLED | — |

---

### 🧩 FPGA & AI Projects

| Project | Work | Links |
|---|---|---|
| FPGA Clock & Stopwatch | FSM-based RTL design and simulation | [Code](https://github.com/twotwoship/fpga_clock) |
| FPGA Microwave | FSM-based RTL design and simulation | [Code](https://github.com/twotwoship/fpga_microwave) · [Demo](https://www.youtube.com/watch?v=djOuPST5QnM) |
| Jetson Rock–Paper–Scissors | Camera input, TensorRT inference, game logic, and Flask/MJPEG streaming | [Code](https://github.com/twotwoship/Assignment_Collection/tree/main/AI_Deep_Learning/%EA%B3%BC%EC%A0%9C%EB%AC%BC%28%EA%B0%80%EC%9C%84%EB%B0%94%EC%9C%84%EB%B3%B4%29) |

The Jetson project has code and models available. Setup instructions and test results still need to be documented.

---

### 📖 What I'm Studying

#### BeagleBone Black Bootloader

- **So far:** ARM startup code, stack/BSS initialization, watchdog disable, UART0, and MLO build.
- **Next:** Boot testing on the board, exception handling, GPIO, DDR initialization, and second-stage loading.

[Code & Notes](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/bbb/bbb-myrtos-stage1)

#### AES-XTS Storage on Zynq

- **So far:** Architecture planning and study of AES-XTS and verification methods.
- **Goal:** Connect Linux driver MMIO, DMA, and IRQ handling to an FPGA encryption path.
- **Status:** Planning stage; implementation and verification are not complete.

[Project Plan](https://github.com/twotwoship/mini_Cipher_Fabric) · [Study Notes](https://github.com/twotwoship/Assignment_Collection/tree/main/aes_xts_nand)

---

### 🎓 Training

**KCCI Seoul Technical Education Center**  
AI System Semiconductor Software Developer Program · Cohort 2

---

### 📁 Practice & Study Folders

[AVR](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128) ·
[STM32](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/stm32) ·
[FPGA](https://github.com/twotwoship/Assignment_Collection/tree/main/fpga) ·
[Data Structures](https://github.com/twotwoship/Assignment_Collection/tree/main/DataStructure) ·
[Algorithms](https://github.com/twotwoship/Assignment_Collection/tree/main/Algorithms) ·
[TCG Opal](https://github.com/twotwoship/Assignment_Collection/tree/main/tcg_storage_opal)

[STM32 Motor Control Exercise](https://github.com/twotwoship/stm_motor_260724) — DC motor speed and direction control using buttons and UART.
