
## 상공회의소 서울기술교육센터 AI 시스템 반도체 SW개발자 2기 프로젝트 작업물

## Projects
| 목차 | 기술 | 역할 | 링크 |
|---|---|---|---|
| **PCB 자동 검사 및 불량 배출 시스템 - Board Guard** | Jetson Orin Nano, STM32F411RE, Python, C, GPIO, EXTI | PCB 존재 판단 및 검사 상태 관리, PASS/FAIL 판정 로직, Jetson-MCU 인터페이스, STM32 상태머신 및 Step/Servo Motor 기반 불량 배출 제어 | [board_guard](https://github.com/clap-min99/board_guard) |
| **Linux Device Driver 기반 Smart Fan** | Jetson Orin Nano, Linux Kernel, C, GPIO, I2C, Character Device Driver | LCD Device Driver, Rotary Encoder Device Driver 담당. `read()`, `write()`, `poll()`, interrupt, FIFO 기반 장치 제어 구현 | [Linux_fan](https://github.com/twotwoship/Linux_fan) |
| **자동 복약 지원 시스템 - 약SLOT** | Raspberry Pi 4, Python, Flask, SQLite, UART, ATmega128A | 팀장 / 시스템 아키텍처 및 통합 담당. Raspberry Pi SW, Flask/SQLite, LCD UI/UX, 음성 안내, UART 통신 관리 담당 | [2026ESWContest_free_YAKSLOT](https://github.com/twotwoship/2026ESWContest_free_YAKSLOT) |
| **STM32 Home IoT Wall Pad** | STM32, C, ADC, PWM, UART, Sensor, DC/Stepper Motor | STM32 센서·입력 및 출력·액추에이터 담당. 온습도/초음파/조도 측정, ADC, DC Motor, Step Motor, LED, Buzzer, Timer/PWM 구현 | [home_plus](https://github.com/twotwoship/home_plus) |
| **STM32 Motor Control** | STM32F411RE, C, UART, PWM | Button/UART/PWM 기반 DC Motor 속도 및 방향 제어 구현 | [stm_motor_260724](https://github.com/twotwoship/stm_motor_260724) |
| **FPGA Stopwatch / Clock** | Verilog HDL, Vivado, FPGA | FSM 기반 Stopwatch/Clock RTL 설계 및 시뮬레이션 | [fpga_clock](https://github.com/twotwoship/fpga_clock) |
| **ATmega128A 디지털시계·계산기** | ATmega128A, C | DS1307 I²C 통신, LCD1602, 키패드, 상태 전환, 스택 기반 수식 계산 | [디지털시계·계산기](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/15.RTC_LCD_calculator_)[시연](https://youtu.be/GpRSiZHryco?si=fW76iYnFrm56GxAn) [발표영상](https://youtu.be/l9xRuBvnAL4?si=AutO_3DuwMXs6mGB) |
| **ATmega128A 장애물 회피·Bluetooth 차량** | ATmega128A, C | 초음파 센서, 외부 인터럽트, PWM 모터 제어, UART/Bluetooth, 수동·자동 주행 FSM | [AUTO_CAR](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/08.auto_car) |
| **ATmega128A 세탁기** | ATmega128A, C | FSM 기반 세탁·헹굼·탈수 상태 전환, 타이머, PWM, UART 명령, FND 표시 | [세탁기](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/07.washing_machine) |
| **Jetson 2인 가위바위보·웹 스트리밍** | Jetson Orin Nano, Python, yolo11n | TensorRT 추론, 카메라 처리 | [가위바위보](https://github.com/twotwoship/Assignment_Collection/tree/main/AI_Deep_Learning/%EA%B3%BC%EC%A0%9C%EB%AC%BC(%EA%B0%80%EC%9C%84%EB%B0%94%EC%9C%84%EB%B3%B4)) |
| **AVR 고추건조기** | C | 고추건조기 | [고추건조기]() |

---

## 학습 중
| 목차 | 기술 | 역할 | 링크 |
|---|---|---|---|
| **Zynq 기반 Linux–FPGA AES-XTS 보안 저장장치** | Verilog HDL, Vivado, FPGA, Linux, C | AES_XTS 하드웨어 가속기, 데이터 및 제어 I/F, Crypto Driver, 계층형 key 관리, 개발 진단 콘솔, DT, PD, MMIO. IRQ, DMA, AXI  | [보안 저장장치]() |

---

## YOUTUBE   
### Practical Training Videos  
https://youtube.com/@twotwoship

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


