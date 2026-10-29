# 이양배 | 포트폴리오

MCU 제어, Linux 디바이스 드라이버, 장치 간 통신을 구현한 프로젝트 모음입니다.

상공회의소 서울기술교육센터 · AI 시스템 반도체 SW개발자 2기  
[GitHub](https://github.com/twotwoship) · [YouTube 시연 모음](https://www.youtube.com/@twotwoship)

## 리눅스 프로젝트

### Board Guard | PCB 자동 검사·불량 배출 시스템

- **담당 역할:** PCB 존재 여부·검사 상태 관리, PASS/FAIL 판정 로직, Jetson–STM32 인터페이스, 상태머신 기반 Step·Servo Motor 배출 제어
- **주요 기술:** C · Python · STM32F411RE · Jetson Orin Nano
- **자료:** [코드·상세 설명](https://github.com/clap-min99/board_guard) · [시연](https://www.youtube.com/shorts/YxTkjOZ_dDI)

### Smart Fan | Linux 디바이스 드라이버 기반 선풍기

- **담당 역할:** LCD·로터리 인코더 드라이버 구현. 인터럽트·FIFO와 `read()`, `write()`, `poll()`을 활용한 장치 입출력 처리
- **주요 기술:** C · Linux Kernel · GPIO · I²C · Jetson Orin Nano
- **자료:** [코드·상세 설명](https://github.com/twotwoship/Linux_fan) · [시연](https://www.youtube.com/shorts/Q2UGQjDw2Ow)

### 약SLOT | 자동 복약 지원 시스템

- **담당 역할:** 팀장·시스템 아키텍처 및 통합 담당. Raspberry Pi SW, Flask·SQLite, LCD UI, 음성 안내, UART 통신 관리
- **주요 기술:** Python · Raspberry Pi 4 · Flask · SQLite · UART
- **자료:** [코드·상세 설명](https://github.com/twotwoship/2026ESWContest_free_YAKSLOT) · [시연](https://www.youtube.com/watch?v=20yo6nAbsLM)

## MCU 프로젝트

- **[STM32 Home IoT 월패드](https://github.com/twotwoship/home_plus)**  
  온습도·초음파·조도 센서 입력과 DC·Stepper Motor, LED, Buzzer 제어 담당  
  `STM32` · `C` · `ADC` · `Timer/PWM` · [시연](https://youtube.com/shorts/tmq6LoPIN-Y?si=yGTwd4x-6AmtZ_au)

- **[ATmega128A 디지털시계·계산기](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/15.RTC_LCD_calculator_)**  
  DS1307 I²C 통신, LCD·키패드 연동, 모드 전환, 스택 기반 수식 계산  
  `ATmega128A` · `C` · `I²C` · `FSM` · [시연](https://www.youtube.com/watch?v=GpRSiZHryco) · [발표](https://www.youtube.com/watch?v=l9xRuBvnAL4)

- **[ATmega128A 장애물 회피·Bluetooth 제어 차량](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/998.auto_car_ver1)**  
  초음파·외부 인터럽트 기반 거리 측정, PWM 모터 제어, 수동·자동 주행 상태 관리  
  `ATmega128A` · `C` · `UART/Bluetooth` · `FSM` · [시연](https://www.youtube.com/shorts/NwpaM5rJNO4)

- **[ATmega128A 세탁기 제어 시스템](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128/07.washing_machine)**  
  세탁·헹굼·탈수 상태 전환, 타이머 기반 시간 관리, PWM 모터·UART 명령 처리  
  `ATmega128A` · `C` · `Timer/PWM` · `FSM` · [시연](https://www.youtube.com/shorts/kGAorJIMjIk)

- **[STM32 고추건조기]()**  
  작성 중
  `STM32` · `C` · `Timer/PWM` · `FSM` · [시연]()


## 추가 프로젝트·실습

<details>
<summary><strong>FPGA·온디바이스 AI 프로젝트</strong></summary>

- **[FPGA Stopwatch / Clock](https://github.com/twotwoship/fpga_clock)**  
  FSM 기반 시계·스톱워치 RTL 설계 및 시뮬레이션  
  `Verilog HDL` · `Vivado` · `FPGA`

- **[FPGA Microwave](https://github.com/twotwoship/fpga_microwave)**  
  FSM 기반 전자레인지 RTL 설계 및 시뮬레이션  
  `Verilog HDL` · `Vivado` · `FPGA` · [시연](https://www.youtube.com/watch?v=djOuPST5QnM)

- **[Jetson 기반 2인 가위바위보 인식·웹 스트리밍](https://github.com/twotwoship/Assignment_Collection/tree/main/AI_Deep_Learning/%EA%B3%BC%EC%A0%9C%EB%AC%BC(%EA%B0%80%EC%9C%84%EB%B0%94%EC%9C%84%EB%B3%B4))**  
  카메라 입력·TensorRT 추론, 게임 상태 처리, Flask/MJPEG 웹 스트리밍 코드 구현  
  `Python` · `Jetson Orin Nano` · `TensorRT` · `Flask`  
  상태: 로컬 코드·모델 보유, 실행 환경 및 동작 검증 결과 정리 필요

</details>

<details>
<summary><strong>MCU 주변장치·통신 실습</strong></summary>

- **[STM32 Motor Control](https://github.com/twotwoship/stm_motor_260724)**  
  버튼·UART 입력에 따른 DC Motor 속도·방향 제어  
  `STM32F411RE` · `C` · `UART` · `PWM`

</details>

<details>
<summary><strong>기초 학습·코드 모음</strong></summary>

- [AVR](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/atmega128): 주변장치·인터럽트·통신 실습
- [STM32](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/stm32): Cortex-M 기반 MCU 실습
- [FPGA](https://github.com/twotwoship/Assignment_Collection/tree/main/fpga): Verilog RTL 실습

</details>

## 개인 공부

### BeagleBone Black 초기 부트로더

- **현재:** ARM 시작 코드, 스택·BSS 초기화, Watchdog 비활성화, UART0 제어 코드와 MLO 빌드 산출물 확보
- **다음 단계:** 실기 부팅 확인, 예외 처리·GPIO 확장, DDR 초기화 및 두 번째 단계 로딩
- **자료:** [코드·개발 범위](https://github.com/twotwoship/Assignment_Collection/tree/main/Embedded/bbb/bbb-myrtos-stage1)

### Zynq 기반 Linux–FPGA AES-XTS 보안 저장장치

- **현재:** 시스템 구조 설계, AES-XTS 및 검증 방식 사전 학습
- **개발 목표:** Linux 드라이버의 MMIO·DMA·IRQ 처리와 FPGA 암호화 데이터 경로 통합
- **자료:** [프로젝트 설계](https://github.com/twotwoship/Assignment_Collection/tree/main/fpga_base_secure_storage_controller_datapath_proto) · [AES-XTS 학습 자료](https://github.com/twotwoship/Assignment_Collection/tree/main/aes_xts_nand)

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

