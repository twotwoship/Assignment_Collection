## 1. C / C++ 언어 ★

- 자료형, 정수 승격, 형변환
- signed / unsigned
- 포인터, 이중 포인터, void 포인터
- 배열과 포인터 차이
- 함수 포인터, 콜백 함수
- `const`, `volatile`, `static`, `extern`
- Scope, Lifetime, Linkage
- Stack / Heap / Data / BSS / Text
- 동적 메모리 할당 (`malloc`, `calloc`, `realloc`, `free`)
- 메모리 누수, Dangling Pointer, Buffer Overflow
- 구조체, 공용체, 열거형
- 구조체 Padding, Alignment
- Bit Field, Bit Mask, Bitwise 연산
- 매크로, 전처리기, `inline`
- Endianness
- Undefined Behavior
- 컴파일, 어셈블, 링크
- C++: 클래스, 생성자, 소멸자, 가상 함수, RAII, 스마트 포인터

---

## 2. 자료구조 / 알고리즘 ★

- 배열, 연결 리스트
- Stack, Queue, Circular Queue
- Ring Buffer, FIFO
- 단일 연결 리스트, 이중 연결 리스트
- Hash Table
- Tree, Binary Tree
- Heap 자료구조
- 정렬: Bubble, Selection, Insertion, Quick, Merge
- 탐색: Linear Search, Binary Search
- 시간 복잡도, 공간 복잡도, Big-O
- 고정 크기 메모리 풀 (Memory Pool)
- Producer–Consumer 구조

---

## 3. 컴퓨터 구조 / 메모리 ★

- CPU, ALU, Register
- Program Counter, Stack Pointer
- Instruction Cycle
- Interrupt, Exception
- Interrupt Vector Table
- Memory Map
- Flash, SRAM, DRAM, ROM
- MMIO (Memory-Mapped I/O)
- Memory Alignment
- Cache, Cache Coherency
- MMU, MPU
- Virtual Memory, Physical Memory
- DMA
- Memory Barrier
- Bus Architecture (AHB, APB, AXI)
- Boot Sequence

---

## 4. MCU / 하드웨어 제어 ★

- GPIO, Input / Output
- Pull-up / Pull-down
- Interrupt / Polling
- ISR, Interrupt Latency
- Interrupt Priority, Nested Interrupt
- Timer / Counter
- PWM
- ADC / DAC
- Watchdog Timer
- Clock, PLL
- Debounce
- Register-Level Programming
- Read-Modify-Write
- Critical Section
- FSM (Finite State Machine)
- State Transition
- Bare-metal Programming

---

## 5. 통신 프로토콜 ★

| 프로토콜 | 핵심 개념 |
|---|---|
| UART | Baud Rate, Start/Stop Bit, Parity, Flow Control |
| SPI | SCLK, MOSI, MISO, CS, CPOL, CPHA |
| I2C | SDA, SCL, Address, ACK/NACK, Clock Stretching |
| CAN | Arbitration, Frame, ID, Bit Stuffing |
| USB | Endpoint, Enumeration, Transfer Type |
| Ethernet | MAC, PHY, Frame |
| TCP/IP | IP, TCP, UDP, Socket |

### 공통 학습 항목

- 동기 / 비동기 통신
- Full-Duplex / Half-Duplex
- Master / Slave, Controller / Target
- 데이터 프레이밍
- CRC, Checksum, Parity
- Timeout, Retry
- Interrupt / DMA 기반 데이터 송수신

---

## 6. 운영체제 / RTOS ★

- Process / Thread / Task
- Context Switching
- Scheduling
- Preemptive / Cooperative Scheduling
- Task Priority
- Mutex / Semaphore
- Spinlock
- Race Condition
- Deadlock
- Priority Inversion / Priority Inheritance
- Critical Section
- Atomic Operation
- IPC (Inter-Process Communication)
- Message Queue, Event Flag
- Task Synchronization
- Determinism
- Deadline, Latency, Jitter
- WCET (Worst-Case Execution Time)
- Stack Overflow
- Watchdog

---

## 7. Embedded Linux / BSP / Device Driver ★
- Linux Kernel / User Space
- System Call
- Kernel Module (`.ko`)
- Character Device / Block Device
- Device File (`/dev`)
- `open()`, `read()`, `write()`, `ioctl()`, `poll()`, `close()`
- `file_operations`
- `platform_device`, `platform_driver`
- Device Tree (DTS / DTB)
- MMIO, `ioremap()`
- Interrupt Handler, `request_irq()`
- Wait Queue, Workqueue
- Kernel Thread
- Mutex, Spinlock, Atomic API
- Kernel Memory Allocation (`kmalloc`, `vmalloc`)
- DMA, DMAEngine
- Cache Coherency, Memory Ordering
- Linux Scheduling
- Virtual Memory, Page Table
- Bootloader (U-Boot)
- Root Filesystem
- Cross Compilation
- Kernel Build / Module Build

---

## 8. 임베디드 개발 환경 / 디버깅 ★

- GCC, Clang
- Make, CMake
- Cross Compiler, Toolchain
- Linker Script
- ELF, Map File
- `objdump`, `readelf`, `nm`
- GDB
- JTAG, SWD
- UART Console
- Oscilloscope
- Logic Analyzer
- `dmesg`, `journalctl`
- `strace`, `gdb`
- Git, Branch, Merge, Rebase
- Debug / Release Build
- Compiler Optimization (`-O0`, `-O2`, `-Os`)
- Warning Options (`-Wall`, `-Wextra`)

---

## 9. 소프트웨어 설계 / 검증 ★

- FSM 설계
- Event-Driven Architecture
- Blocking / Non-blocking
- Synchronous / Asynchronous
- Callback
- Timeout / Error Handling
- Resource Management
- Unit Test
- Integration Test
- Hardware-in-the-Loop (HIL)
- Boundary Value Test
- Fault Injection
- Assert
- Coding Standard (MISRA C 등)
- Static Analysis
- Memory Leak Detection
- Regression Test

---

## 10. 직무별 심화 ☆

| 지원 분야 | 추가 학습 |
|---|---|
| MCU Firmware | ARM Cortex-M, NVIC, CMSIS, HAL/LL, Startup Code |
| Embedded Linux / BSP | U-Boot, Device Tree, Yocto, Buildroot, Kernel Porting |
| Linux Driver | MMIO, IRQ, DMA, Concurrency, Driver Model |
| FPGA / SoC SW | AXI4-Lite, AXI4-Stream, DMA, BRAM, PS/PL |
| 차량 전장 | CAN / CAN-FD, UDS, AUTOSAR, ISO 26262 |
| IoT | BLE, Wi-Fi, MQTT, TLS |
| 로보틱스 | Motor Control, Encoder, PID, ROS 2 |
| 방산 / 산업제어 | RTOS, Real-Time Ethernet, Reliability, Safety |

---

### 주의할 개념

- `volatile`과 원자성 차이
- ISR 진입/복귀와 태스크 컨텍스트 스위칭 차이
- `packed` 구조체와 메모리 정렬 문제
- Mutex와 Semaphore 차이
- Spinlock과 Mutex 차이
- Stack과 Heap 차이
- MMIO와 일반 메모리 접근 차이
- Cache Coherency와 Memory Barrier 차이
- Interrupt와 Polling 차이
- Blocking과 Non-blocking 차이



#### 질문

* 버스와 통신의 차이는?
  1. 
* 버스란 무엇인가?
메모리 버스란?
* 통신이란 무엇인가?

* 직렬통신과 병렬통신의 차이는?
  1. 직렬통신 :
  2. 병렬통신 : 

* 동기와 비동기란?
* 채터링이란?
* DMA란?
* STACK 이란?
* Queue 란?
* 후위변환이란?
* 후위계산이란?
* 포인터란? / 고급포인터란? 포인터의 역할은?
* 분주를 하는 이유?
* 폴링과 인터럽트 차이
* c언어를 사용하는 이유
* c언어의 빌드 과정
* arm cpu의 부팅 과정
* 멀티프로세서와 멀티프로세싱의 차이는?
* 프로세스와 쓰레드의 차이는?
* 추상화란?
* volatile static const 의 사용 이유
* c언어로 하드웨어 레지스터를 직접 접근해서 통제하는 방법은? 그리고 그렇게 하는 이유는?
* 주파수 클럭이란?
* RAM과 ROM에 대해 설명을 할수 있냐?
* DRAM은? CPU가 들어간 DRAM에 대해 설명해라.
* 풀업과 풀다운 / 푸시풀 / 오픈드레인에 대해 각각 구체적으로 어떻게 동작을 하는지 설명 할 수 있는가? 그리고 왜 사용하는지도 설명이 가능한가?
* 반도체 npn pnp 트랜지스터란?
* OSI 7계층 TCP/IP 4계층에 대해 설명이 가능한지?
* 메모리 주소번지를 계산할 수 있는가? CPU별로 어떻게 계산을 해야 되는가?
* ISR 이란? IRQ란?
* 문맥교환이란? 문맥교환은 어떻게 이루어지며 언제 왜 이루어 지는가?
* 멀티쓰레드와 멀티코어의 차이는 무엇이며 실제로 어떤 부분이 다르게 작동하는지?
* 부트로더와 커널과 OS(리눅스)의 차이와 역할은?
* 상위설계서를 작성하는 이유는? 어떻게 작성해야 하는가?
* 크로스컴파일이란? 크로스컴파일 환경은 어떻게 구성하는지? 크로스컴파일을 왜 하는지?
* 툴체인이란?
* GPU CPU NPU 차이는?
* 파싱이란?
* 연결리스트란?
* 

#### 배운 통신 프로토콜
몇가닥을 사용하는지? 사용되는 클럭은? 단방향/양방향/전이중인지? 해당 프로토콜에서 사용하는 통신 규약은?

* I2C
* SPI
* CAN
* UART/USART
* USB
* UART-USB 연결은 어떻게 되는건지?
