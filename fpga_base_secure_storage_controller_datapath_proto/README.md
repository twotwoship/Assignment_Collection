# FPGA 기반 AES-XTS Hardware Accelerator의 Linux HW/SW 통합

### [개발 배경 및 문제 발견]  
현대 SSD에서는 저장 데이터 보호를 위해 hardware encryption을 지원하는 제품이 존재  
하지만 시스템 관점에서는 암호화 정책이 `SSD` 내부 또는 `HOST / SOC` 에 존재 할 수 있다.  
`SoC/embedded system`에서는 저장장치 제조사와 관계없이 시스템 자체의 `security policy`로 `storage encryption`을 관리하고 싶은 요구가 있을 수 있습니다.
상용 SSD 자체 암호화에 의존하면 암호화 구현과 Key Management의 신뢰 경계가 SSD Controller 내부에 존재  

### [개발 목적]
1. Linux 기반 시스템에서 전용 AES-XTS FPGA 가속기를 Device Driver와 DMA를 통해 제어
2. CPU에서 수행되던 저장 데이터 암·복호화를 Hardware로 Offload하는 HW/SW 통합 시스템을 구현
3. SSD 제조사의 SED 기능에 의존하지 않고 system-level security boundary를 구성

### [아이디어]  
데이터가 SSD에 진입하기 전에
Host-controlled Key를 이용하여 암호화

### [핵심]
1. 리눅스 드라이버
```
Linux Device Driver
        │
        ├─ FPGA register
        ├─ DMA
        ├─ interrupt
        └─ user interface
```   
2. cpu와 fpga의 역할 분리 측정
3. 실제 i/o 단위 사용

### [증명]  
CPU에서 AES-XTS를 수행하면
CPU 자원과 Memory bandwidth를 소비

### [해결]  
Linux dm-crypt  
FPGA에 AES-XTS Hardware Pipeline을 구현한다.

### [구조]  
Linux -> Driver -> AXI DMA -> FPGA AES-XTS -> Encrypted Storage

### [특징]  
LBA 기반 Tweak,  Host-managed Key,  Inline Encryption,  CPU Offload  
Cryptographic Erase  

### [개발 목표]
```
CPU에서 수행할 수 있는 AES-XTS 연산
              ↓
전용 FPGA hardware로 offload
              ↓
Linux Device Driver가 제어
              ↓
DMA로 대용량 데이터 전달
```

### [구현 목표]

#### SSD Controller 내부 inline encryption datapath를 참고하여 FPGA 기반 storage crypto accelerator를 구현하고 Linux device driver와 DMA를 통해 HW/SW co-design을 검증

```
Linux Software
Device Driver
DMA / Register / Interrupt
FPGA Hardware IP
```

```
WRITE I/O
 │
 ├── LBA
 ├── Length
 ├── Key
 └── Data
       │
       ▼
     AES-XTS
       │
       ▼
Encrypted I/O
```

### [검증 및 증명]  
0. linux applictation[software, driver]
1. dma
2. OpenSSL Golden Model      =      FPGA  
3. 다른 LBA에 같은 평문 저장 후 암호문 비교 X != Y
4. CPU software 대비 throughput/CPU load 비교

### [성능]
|측정|비교군|개발|
|---|---|---|
|---|CPU Software AES-XTS%|FPGA AES-XTS|
|speed|---|---|
|Throughput|---|---|
|Latency|---|---|
|CPU load|---|---|
|FPGA resource usage|---|---|

### [응용]   
SSD Controller IP
Secure Storage Appliance
Military / Edge Storage
Data Center Storage

### [의미]
`AES → XTS → LBA → DMA → FPGA → Linux Driver → SSD`  
FPGA는 SSD의 AES를 대체하기 위한 것이 아니라 "향후 ASIC SSD Controller에 탑재할 Secure Storage Datapath를 검증하는 Hardware Prototype"
```
                Secure Storage
                       │
             ┌─────────┴──────────┐
             │                    │
        Storage semantics       Security
             │                    │
            LBA                AES-XTS
             │                    │
             └─────────┬──────────┘
                       │
                Hardware datapath
                       │
                     FPGA
                       │
                  CPU offload
                       │
                    AXI DMA
                       │
                 OS integration
                       │
                Linux Driver
```

## [정리]
Linux의 block I/O 데이터를 DMA로 FPGA에 전달하고, FPGA에서 LBA 기반 AES-XTS를 inline으로 처리하는 저장장치 암호화 하드웨어 prototype
### 요약
```
Application
    ↓
Linux Kernel
    ↓
Device Driver
    ↓
MMIO / Interrupt / DMA
    ↓
Hardware IP
```

### [상세]
#### [Linux에서 하드웨어 암호 엔진을 어떻게 제어하고, 데이터를 어떻게 전달하고, 완료를 어떻게 받고, SW 방식과 어떻게 검증했는가?]
```
┌──────────────────────────────┐
│ Linux                        │
│                              │
│ Application                  │
│     ↓                        │
│ File System                  │
│     ↓                        │
│ Block I/O                    │
└─────┬────────────────────────┘
      │
      ▼
┌──────────────────────────────┐
│ Linux Device Driver          │
│                              │
│ FPGA 제어                    │
│ Key 설정                     │
│ DMA 설정                     │
└─────┬────────────────────────┘
      │
      ▼
┌──────────────────────────────┐
│ AXI DMA                      │
│                              │
│ Memory ↔ FPGA data transfer │
└─────┬────────────────────────┘
      │
      ▼
╔══════════════════════════════╗
║ FPGA                         ║
║                              ║
║ LBA                          ║
║  │                           ║
║  ▼                           ║
║ Tweak Generator              ║
║  │                           ║
║  ▼                           ║
║ AES-XTS Engine               ║
║  │                           ║
║  ▼                           ║
║ Ciphertext                   ║
╚═════╤════════════════════════╝
      │
      ▼
   Storage
```

#### [Linux driver]
```
CONTROL register 설정
       ↓
LBA 전달
       ↓
Key 전달
       ↓
DMA buffer 준비
       ↓
DMA 시작
       ↓
FPGA 동작
       ↓
Interrupt 발생
       ↓
Driver가 완료 처리
       ↓
User에게 결과 반환
```

### [DATA FLOW]
WRITE
```
Application
     │
     ▼
Linux File System
     │
     ▼
Block I/O
     │
     │ LBA = 1000
     ▼
Linux Driver
     │
     │ DMA 설정
     ▼
AXI DMA
     │
     │ 4096B
     ▼
FPGA
     │
     ├─ LBA1000 → Tweak
     │
     ├─ AES-XTS
     │
     ▼
Ciphertext
     │
     ▼
Storage
```
READ
```
Storage
   │
   │ Ciphertext
   ▼
FPGA
   │
   ├─ LBA1000 → Tweak
   │
   ├─ AES-XTS decrypt
   │
   ▼
DMA
   │
   ▼
RAM
   │
   ▼
Linux
   │
   ▼
Application
```
