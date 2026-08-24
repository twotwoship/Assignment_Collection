# AES-XTS + AXI DMA + Linux Driver + FPGA 프로젝트 학습·실습 운영 계획

## 1. 전체 전략

현재 남은 교육과정은 다음과 같다.

| 구간 | 기간 | 핵심 목표 |
|---|---:|---|
| 사전 준비 | 지금부터 4주 | AES-XTS 검증환경 + Linux 기초 + AXI/DMA 개념 |
| Linux Driver 수업 | 3주 | FPGA를 제어할 Linux Driver 준비 |
| FPGA 수업 | 3주 | AES-XTS IP + AXI/DMA 데이터 경로 구현 |
| 최종 프로젝트 | 3주 | 통합 + 디버깅 + 테스트 + 성능측정 + 발표 |

핵심 방향은 다음과 같다.

```text
지금
↓
각 부품을 이해하고 작은 단위로 만들어 놓음

Linux 수업
↓
Driver 부분 완성

FPGA 수업
↓
RTL / AXI / DMA 부분 완성

최종 프로젝트
↓
새 기능 개발 X

이미 만든 것 연결
↓
Test
↓
Bug Fix
↓
Performance 측정
↓
Demo
```

최종 프로젝트 기간에 AES를 처음 구현하거나 Linux Driver를 처음 작성하지 않는 것을 목표로 한다.

---

## 2. 프로젝트를 4개 덩어리로 분리

전체 시스템을 한꺼번에 만들지 않고 다음 네 부분으로 나눈다.

```text
① Golden Model
   OpenSSL / C
        │
        │ 정답 데이터
        ↓
② Linux Application
        │
        ↓
③ Linux Driver
        │
        ↓
④ FPGA
   AXI DMA
      ↓
   AES-XTS RTL
```

최종 데이터 흐름은 다음과 같다.

```text
파일 / Test Data
      │
      ↓
Linux Application
      │
   read/write
   ioctl
      │
      ↓
Linux Driver
      │
      ↓
DMA Buffer
      │
      ↓
AXI DMA
      │
      ↓
AES-XTS FPGA
      │
      ↓
Encrypted Data
```

즉, 프로젝트의 본질은 이 하나의 데이터 경로를 완성하고 검증하는 것이다.

---

# 3. 사전 준비 4주

현재 4주는 FPGA만 깊게 공부하지 않는다.

권장 시간 배분:

| 분야 | 비중 |
|---|---:|
| AES/AES-XTS + Golden Model | 30% |
| Linux/C/Linux Driver 사전 기초 | 30% |
| AXI/AXI DMA 개념 | 20% |
| FPGA/RTL 기초 | 20% |

---

## 4. 사전준비 1주차 — AES-XTS 기준점 만들기

### 목표

FPGA 결과가 맞는지 판단할 수 있도록 먼저 정답 데이터를 생성하는 환경을 만든다.

### 공부

AES 기본 개념:

```text
Plaintext
Ciphertext
Key
AES-128 / AES-256
Block = 128 bit
Encrypt
Decrypt
```

AES-XTS 기본 개념:

```text
Data Unit
LBA
Tweak
Key1
Key2
AES-XTS Encrypt
AES-XTS Decrypt
```

특히 다음 관계를 이해한다.

```text
LBA
 ↓
Tweak 계산
 ↓
AES-XTS
 ↓
Sector 암호화
```

### 실습

OpenSSL을 사용하여 AES-XTS Golden Vector를 만든다.

예:

```text
Plaintext
= 512 byte

Key
= 고정

LBA
= 0

↓ AES-XTS

Ciphertext
= 정답
```

복호화도 수행한다.

```text
Ciphertext
↓
AES-XTS Decrypt
↓
원본 Plaintext
```

### 통과 조건

```text
plaintext.bin
      ↓
OpenSSL AES-XTS
      ↓
cipher.bin

cipher.bin
      ↓
Decrypt
      ↓
result.bin

cmp plaintext.bin result.bin
```

결과:

```text
MATCH
```

이 환경을 이후 FPGA 검증의 Golden Model로 사용한다.

---

## 5. 사전준비 2주차 — Linux Application과 Driver 기초

FPGA가 없어도 Linux 부분은 먼저 준비한다.

### 공부

```text
User Space
Kernel Space
System Call

open()
read()
write()
close()

Device File
Character Device Driver
ioctl()
mmap()
Kernel Module
```

기본 흐름:

```text
Application
     │
     │ open()
     │ write()
     │ ioctl()
     ↓
/dev/xts_accel
     │
     ↓
Linux Driver
```

### 실습

FPGA 없이 가짜 장치를 만든다.

예:

```text
/dev/xts_accel
```

Application 구조:

```c
fd = open("/dev/xts_accel", O_RDWR);

write(fd, buffer, 4096);

ioctl(fd, SET_KEY, ...);

read(fd, buffer, 4096);
```

초기에는 Driver에서 단순 복사만 수행해도 된다.

```text
write()
↓
kernel buffer에 memcpy

read()
↓
다시 application으로 전달
```

### 통과 조건

```text
Application
↓
Linux Driver
↓
Kernel Buffer
↓
Linux Driver
↓
Application
```

데이터가 깨지지 않고 왕복해야 한다.

---

## 6. 사전준비 3주차 — AXI와 DMA

### 반드시 알아야 할 개념

```text
AXI4
AXI4-Lite
AXI4-Stream
DMA
MM2S
S2MM
DDR
BRAM
FIFO
```

핵심 데이터 흐름:

```text
DDR
 │
 │ MM2S
 ↓
AXI DMA
 │
 │ AXI4-Stream
 ↓
AES-XTS
 │
 │ AXI4-Stream
 ↓
AXI DMA
 │
 │ S2MM
 ↓
DDR
```

### MM2S

```text
Memory Mapped → Stream
```

DDR의 데이터를 FPGA 회로 쪽으로 전달한다.

### S2MM

```text
Stream → Memory Mapped
```

FPGA가 처리한 데이터를 다시 DDR에 기록한다.

---

## 7. 사전준비 4주차 — FPGA/RTL 기초

### 공부

```text
RTL
Register
Clock
Reset
FSM
Valid
Ready
FIFO
AXI Stream
```

### 가장 먼저 할 실습

AES보다 먼저 DMA Loopback 구조를 이해한다.

```text
DDR
 ↓
DMA
 ↓
FPGA
 ↓
그대로 출력
 ↓
DMA
 ↓
DDR
```

FPGA 내부는 처음에는 단순 통과 구조여도 된다.

```text
input_data
   ↓
그대로
   ↓
output_data
```

이 단계를 먼저 성공시키는 이유는 이후 문제가 발생했을 때 원인을 분리하기 위해서이다.

```text
DMA 문제인지
AES 문제인지
AXI 문제인지
Driver 문제인지
```

Loopback이 정상이라면 DMA/AXI/DDR 경로가 정상이라는 근거가 된다.

---

# 8. 사전 4주 종료 목표

```text
Week 1
AES-XTS Golden Model
       ✓

Week 2
Linux App ↔ Dummy Driver
       ✓

Week 3
AXI/DMA 이해
       ✓

Week 4
RTL + Stream/Loopback 이해
       ✓
```

---

# 9. Linux Driver 수업 3주

권장 비율:

```text
20% 이론
70% 실습
10% 프로젝트 정리
```

수업에서 배운 예제를 별도 예제로 끝내지 않고 즉시 프로젝트 코드로 변환한다.

---

## 10. Linux 1주차 — Character Device

수업 예제:

```text
/dev/mydevice
```

프로젝트 버전:

```text
/dev/xts_accel
```

### 목표

다음 함수가 정상 동작하도록 한다.

```text
open
release
read
write
ioctl
```

---

## 11. Linux 2주차 — FPGA 제어 인터페이스 설계

예시 ioctl:

```text
ioctl(SET_KEY)
ioctl(SET_LBA)
ioctl(START_ENCRYPT)
ioctl(START_DECRYPT)
```

예시 구조체:

```c
struct xts_request {
    uint64_t lba;
    uint32_t length;
    uint32_t direction;
};
```

이 시점에는 FPGA 구현보다 Application ↔ Driver 인터페이스를 명확히 확정하는 것이 중요하다.

---

## 12. Linux 3주차 — DMA와 IRQ

### 공부

```text
DMA Buffer
Physical Address
Virtual Address
DMA Mapping
Interrupt
IRQ
Memory Mapped I/O
ioremap
readl
writel
```

핵심 흐름:

```text
Application
 ↓
Driver
 ↓
DMA Buffer
 ↓
FPGA
 ↓
Interrupt
 ↓
Driver
 ↓
Application
```

### Linux 과정 종료 시 최소 산출물

```text
test_app.c
xts_driver.c
xts_ioctl.h
/test
```

---

# 13. FPGA 수업 3주

권장 비율:

```text
이론       20%
RTL 실습   60%
검증       20%
```

---

## 14. FPGA 1주차 — AES Block Cipher

처음부터 AES-XTS 전체를 구현하지 않는다.

순서:

```text
AES Core
↓
Testbench
↓
Golden Vector 비교
```

목표:

```text
C/OpenSSL

Ciphertext:
123456...

FPGA

Ciphertext:
123456...

MATCH
```

---

## 15. FPGA 2주차 — AES-XTS

AES Core 위에 XTS 구조를 추가한다.

```text
AES
+
Tweak
+
GF(2^128)
+
XTS
```

초기 테스트 데이터 크기:

```text
512 byte
4096 byte
```

검증:

```text
OpenSSL
       ↓
Ciphertext A

FPGA
       ↓
Ciphertext B

A == B
```

목표는 bit-exact match이다.

---

## 16. FPGA 3주차 — AXI/DMA 연결

단계별로 진행한다.

### Test 1

```text
DMA → Loopback → DMA
```

### Test 2

```text
DMA → AES → DMA
```

### Test 3

```text
DMA → AES-XTS → DMA
```

처음부터 모든 기능을 연결하지 않는다.

---

# 17. 최종 프로젝트 시작 전 목표 상태

최종 프로젝트 시작 시점에는 아래 항목이 가능한 한 완료되어 있어야 한다.

```text
AES Golden Model
      ✓

AES-XTS Golden Model
      ✓

Linux Test Application
      ✓

Linux Driver 기본 구조
      ✓

DMA 기본 동작
      ✓

AES RTL
      ✓

AES-XTS RTL
      ✓

OpenSSL ↔ FPGA 검증
      ✓
```

가능하면 다음까지 완료한다.

```text
DMA ↔ AES-XTS
      ✓
```

---

# 18. 최종 프로젝트 3주 운영

최종 프로젝트 기간에는 새로운 이론 학습을 최소화한다.

| 작업 | 비중 |
|---|---:|
| 통합 | 30% |
| 테스트/검증 | 35% |
| 디버깅 | 20% |
| 성능측정 | 10% |
| 문서/발표 | 5% |

---

## 19. 최종 프로젝트 1주차 — End-to-End 통합

목표:

```text
Linux
 ↕
Driver
 ↕
DMA
 ↕
FPGA
 ↕
AES-XTS
```

데이터 흐름:

```text
plaintext
↓
Linux App
↓
Driver
↓
DMA
↓
FPGA AES-XTS
↓
DMA
↓
Driver
↓
ciphertext
```

최종 검증:

```text
FPGA ciphertext
==
OpenSSL ciphertext
```

---

## 20. 최종 프로젝트 2주차 — 기능 및 검증 테스트

### 기능 테스트

```text
Encrypt
Decrypt
Encrypt → Decrypt
```

### 데이터 크기 테스트

```text
512 B
4 KB
64 KB
1 MB
10 MB
100 MB
```

### LBA 테스트

```text
LBA 0
LBA 1
LBA 100
LBA 10000
```

### Key 테스트

```text
Key A
Key B
Random Key
```

### XTS 핵심 검증

동일한 Plaintext와 동일한 Key라도 LBA가 다르면 Ciphertext는 달라야 한다.

```text
Plaintext 동일
Key 동일
LBA 다름
```

결과:

```text
Ciphertext 다름
```

---

# 21. 최종 프로젝트 2주차 — 성능 측정

CPU:

```text
OpenSSL AES-XTS
```

FPGA:

```text
AXI DMA + AES-XTS
```

측정 항목:

```text
MB/s
GB/s
Latency
CPU Utilization
```

예시 결과 표:

| 데이터 | CPU | FPGA | 가속 |
|---|---:|---:|---:|
| 4 KB | A | B | x배 |
| 1 MB | A | B | x배 |
| 100 MB | A | B | x배 |

프로젝트 결과물에서는 단순히 “FPGA를 사용했다”가 아니라 실제 성능 차이를 수치로 보여주는 것이 중요하다.

---

# 22. 최종 프로젝트 3주차 — 기능 추가 금지

마지막 주에는 새 기능 개발을 하지 않는다.

```text
Bug Fix
↓
Regression Test
↓
성능 Test
↓
Demo 반복
↓
발표 자료
```

예를 들어 다음과 같은 확장은 마지막 주에 시도하지 않는다.

```text
GCM 추가
GUI 대규모 추가
NVMe 신규 연결
새로운 암호 모드
```

완성도와 재현성을 우선한다.

---

# 23. Crypto Erase Demo

프로젝트의 차별화 요소로 Crypto Erase를 사용할 수 있다.

```text
16 TB Storage
        ↓
모든 데이터를 AES-XTS 암호화
        ↓
Encryption Key 존재
        ↓
정상 복호화 가능
```

그 후:

```text
KEY DESTROY
```

```text
암호문은 그대로 존재

하지만

복호화 Key 없음
        ↓
데이터 사용 불가능
```

기술적으로는 다음과 같이 설명한다.

> 16 TB NAND 전체에 0을 쓰는 것이 아니라, 저장 데이터를 암호화한 키를 안전하게 폐기함으로써 전체 데이터에 대한 실질적인 접근을 매우 짧은 시간 안에 불가능하게 만드는 Crypto Erase 구조를 구현한다.

---

# 24. 하루 4시간 기준 시간 배분

현재 사전 준비 기간:

```text
이론        1시간
실습        2시간 30분
정리/검증   30분
```

즉 대략:

```text
25 : 62.5 : 12.5
```

강의를 오래 듣는 것보다 실제 프로젝트 코드와 테스트를 만드는 시간을 더 길게 가져간다.

---

# 25. Linux 과정 중 하루 4시간 예시

```text
30분
오늘 배운 내용 정리

↓

2시간 30분
수업 예제를 프로젝트 코드로 재작성

↓

1시간
테스트 / 디버깅 / Git 정리
```

기준은 “배웠다”가 아니라 “내 프로젝트에서 동작한다”이다.

---

# 26. FPGA 과정 중 하루 4시간 예시

예를 들어 AXI4-Stream을 배웠다면 단순 복습에서 끝내지 않는다.

```text
AXI Stream Source
        ↓
내 RTL
        ↓
AXI Stream Sink
```

해당 내용을 프로젝트의 `aes_xts_ip` 형태로 직접 옮긴다.

---

# 27. 전체 13주 로드맵

| 주차 | 핵심 |
|---|---|
| 사전 1주 | AES-XTS/OpenSSL Golden Model |
| 사전 2주 | Linux App + 간단한 Driver |
| 사전 3주 | AXI / DMA / DDR / AXI Stream |
| 사전 4주 | Verilog / RTL / FSM / Testbench |
| Linux 1주 | Character Driver |
| Linux 2주 | ioctl / mmap / Register Interface |
| Linux 3주 | DMA / IRQ / MMIO |
| FPGA 1주 | AES RTL |
| FPGA 2주 | AES-XTS RTL |
| FPGA 3주 | AXI Stream + DMA |
| 프로젝트 1주 | End-to-End 통합 |
| 프로젝트 2주 | 검증 + 성능 측정 |
| 프로젝트 3주 | Regression + Demo + 발표 |

---

# 28. 프로젝트 실제 개발 순서

권장 순서는 다음과 같다.

```text
1. OpenSSL AES-XTS
        ↓
2. C Test Program
        ↓
3. Linux App
        ↓
4. Dummy Driver
        ↓
5. FPGA AES
        ↓
6. FPGA AES-XTS
        ↓
7. DMA Loopback
        ↓
8. DMA + AES-XTS
        ↓
9. Linux Driver + DMA
        ↓
10. Linux + DMA + AES-XTS
        ↓
11. 성능 측정
        ↓
12. Crypto Erase Demo
```

이 순서의 목적은 문제 발생 시 고장 난 계층을 빠르게 분리하기 위함이다.

예:

```text
OpenSSL       PASS
AES RTL       PASS
XTS RTL       PASS
DMA Loopback  PASS
DMA + XTS     FAIL
```

이 경우 문제 범위를 다음 정도로 줄일 수 있다.

```text
AXI Stream 인터페이스
또는
XTS ↔ DMA 연결
```

이것이 계층별 검증과 단계별 통합의 핵심이다.

---

# 29. 피해야 할 공부 방식

## FPGA만 30일 집중

```text
Verilog
↓
FSM
↓
Pipeline
↓
Timing
↓
DSP
↓
BRAM
↓
고급 FPGA 최적화
```

FPGA 지식은 늘 수 있지만 최종 프로젝트에서 Linux/Driver/DMA 통합에 막힐 가능성이 크다.

## Linux Driver를 수업 시작 후 처음 접하기

사전 준비 없이 수업 시작과 동시에 Driver를 처음 접하면 프로젝트에 적용할 시간이 부족해진다.

## 최종 프로젝트 3주에 처음 통합하기

가장 위험한 방식이다.

최종 프로젝트 기간에는 이미 만들어 둔 구성 요소를 통합하고 테스트해야 한다.

---

# 30. 기존 30일 커리큘럼 수정안

기존 FPGA 중심 계획보다 다음 구성이 적합하다.

```text
1~7일
AES / AES-XTS / OpenSSL / Golden Model

8~14일
Linux / C / Kernel / Driver 기초

15~20일
AXI / DMA / DDR / AXI Stream

21~25일
Verilog / RTL / FSM / Testbench

26~30일
AES RTL 맛보기
+
전체 프로젝트 Interface 설계
```

---

# 최종 목표

최종 목표는 다음과 같이 잡는다.

> FPGA 수업 종료 시점에 주요 개발을 끝내고, 최종 프로젝트 3주는 Linux Driver + AXI DMA + AES-XTS FPGA 통합 검증과 성능 평가에 집중한다.

단계별 시간 배분:

```text
현재 4주
공부 40 : 실습 50 : 검증 10

Linux 3주
공부 20 : 실습 65 : 검증 15

FPGA 3주
공부 20 : 실습 60 : 검증 20

최종 3주
공부 5 : 개발/통합 35 : 테스트/디버깅 60
```

프로젝트의 완성도를 좌우하는 것은 AES-XTS 알고리즘만 구현하는 것이 아니라 다음 세 가지를 실제로 증명하는 것이다.

1. Linux Driver ↔ DMA ↔ FPGA 경로가 안정적으로 연결되는가.
2. OpenSSL/NIST 기준 결과와 FPGA 결과가 bit-exact하게 일치하는가.
3. CPU 대비 FPGA 가속 성능이 실제 수치로 얼마나 개선되는가.
