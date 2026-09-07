
### [문제]  
상용 SSD 자체 암호화에 의존하면  
암호화 구현과 Key Management의 신뢰 경계가
SSD Controller 내부에 존재한다.

### [아이디어]  
데이터가 SSD에 진입하기 전에
Host-controlled Key를 이용하여 암호화

### [문제]  
CPU에서 AES-XTS를 수행하면
CPU 자원과 Memory bandwidth를 소비

### [해결]  
FPGA에 AES-XTS Hardware Pipeline을 구현한다.

### [구조]  
Linux -> Driver -> AXI DMA -> FPGA AES-XTS -> Encrypted Storage

### [특징]  
LBA 기반 Tweak,  Host-managed Key,  Inline Encryption,  CPU Offload  
Cryptographic Erase  

### 목표
암호 기능을 I/O 경로에 추가했지만 암호기가 storage throughput을 깎아먹지 않도록 충분한 처리량을 확보한다.

### [검증]  
OpenSSL Golden Model      =      FPGA  

### [성능]
|측정|비교군|개발|
|---|---|---|
|---|CPU Software AES-XTS 2 GB/s CPU 80%|FPGA AES-XTS  4 GB/s CPU 10%|
|Throughput|---|---|
|Latency|---|---|
|CPU load|---|---|
|FPGA resource usage|---|---|

### [응용]   
SSD Controller IP
Secure Storage Appliance
Military / Edge Storage
Data Center Storage

### [최종 의미]    
FPGA는 SSD의 AES를 대체하기 위한 것이 아니라  
"향후 ASIC SSD Controller에 탑재할  
Secure Storage Datapath를 검증하는  
Hardware Prototype"


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


