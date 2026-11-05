C 언어
- 자료형: 값 범위·크기·해석 방식 결정. 정수 크기 플랫폼 의존. uint32_t 등 고정 폭 자료형 활용.
- 정수 승격: char, short 등 연산 전 int 또는 unsigned int 변환.
- 형변환: 확대·축소 과정에서 값 범위·부호·정밀도 변화. 명시적 캐스팅도 데이터 손실 발생.
- signed/unsigned: 혼합 연산 시 변환 규칙 적용. 음수의 unsigned 변환으로 비교 결과 변경 가능.
- Overflow: signed 정수 범위 초과 → Undefined Behavior. unsigned 연산 → 표현 범위 기준 순환.
- Undefined Behavior: 언어 규칙상 동작 보장 없음. 배열 범위 초과, 해제 메모리 접근, signed overflow 등.
- sizeof: 객체·자료형 크기 반환. 단위 바이트. 반환형 size_t.
- 매크로: 전처리 단계 텍스트 치환. 인자 괄호·중복 평가 주의.
- inline: 함수 정의·링크 관련 지정. 함수 호출 제거 보장 없음.

포인터
- 포인터: 객체·함수 주소 표현. 대상 자료형에 따라 역참조·포인터 연산 결정.
- 포인터 연산: 대상 자료형 크기 단위 이동. 동일 배열 범위 내 연산 원칙.
- 이중 포인터: 포인터 객체 주소 저장. 함수 내부에서 호출자 포인터 변경 시 사용.
- void *: 객체 주소 전달용 범용 포인터. 역참조 전 대상 자료형 지정 필요.
- 함수 포인터: 함수 주소 저장. 콜백·상태별 처리·드라이버 인터페이스 구현.
- 배열·포인터 차이: 배열은 요소 집합, 포인터는 주소 저장 객체. 배열명은 대부분의 식에서 첫 요소 포인터로 변환.
- 배열 인자: 함수 매개변수의 배열 선언은 포인터로 조정. 배열 길이 별도 전달.

키워드·수명
- const: 해당 접근 경로 통한 값 변경 제한. 객체의 ROM 배치 보장 없음.
- volatile: 관찰 가능한 접근 유지. 하드웨어 레지스터 접근 등에 사용. 원자성·스레드 동기화 보장 없음.
- static 지역 변수: 함수 호출 종료 후 값 유지. 프로그램 실행 기간 저장 공간 유지.
- static 전역 변수·함수: 현재 번역 단위 내부 연결.
- extern: 외부 정의 객체·함수 선언에 사용.
- Scope: 이름 참조 가능 범위.
- Lifetime: 객체 존재 기간.
- Linkage: 서로 다른 선언이 동일 객체·함수를 가리키는 관계.

메모리
- Text: 실행 코드 영역. 읽기 전용 데이터는 보통 별도 .rodata 배치.
- Data: 초기값을 가진 정적 저장 기간 객체 중 쓰기 가능 데이터 배치.
- BSS: 0 초기화 정적 저장 기간 객체 배치. 실행 파일에 전체 초기값 저장 불필요.
- Stack: 함수 호출 정보·지역 변수 등에 사용. 호출 깊이·지역 배열 크기에 따라 사용량 증가.
- Heap: 동적 할당 영역. 할당 실패·단편화·할당 시간 변동 고려.
- malloc/calloc: 메모리 할당. calloc은 할당 영역 바이트를 0으로 초기화.
- realloc: 할당 크기 변경. 주소 이동 가능. 실패 시 기존 할당 유지.
- free: 동적 할당 해제. 중복 해제·해제 후 접근 금지.
- 소유권: 자원 해제 책임. 소유자·사용자·유효 기간 구분.
- Memory Pool: 고정 크기 블록 재사용. 할당 시간 예측·단편화 제한.

메모리 오류
- Buffer Overflow: 버퍼 경계 밖 쓰기. 데이터·제어 정보 손상.
- Dangling Pointer: 수명 종료 객체를 가리키는 포인터.
- Use-After-Free: 해제된 동적 메모리 접근.
- 메모리 누수: 해제 책임 상실·해제 누락으로 메모리 점유 지속.
- Stack Overflow: 스택 용량 초과. 재귀·대형 지역 배열·인터럽트 중첩 점검.

데이터 표현·처리
- 구조체: 멤버별 저장 공간 보유. 멤버 사이·끝부분 Padding 가능.
- 공용체: 멤버 간 저장 공간 공유. 크기는 최대 멤버 크기·정렬 조건 반영.
- 열거형: 정수 상수에 이름 부여. 상태·명령·오류 코드 표현.
- Alignment: 자료형 접근 주소 정렬 조건. 위반 시 성능 저하·Fault 가능.
- Endianness: 다중 바이트 값의 메모리 저장 순서. 통신·파일 형식 변환 시 확인.
- Bit Mask: AND로 검사·클리어, OR로 설정, XOR로 반전.
- Shift: 비트 이동·필드 추출. 이동 횟수·부호·자료형 폭 확인.
- 문자열: C 문자열 끝은 '\0'. 저장 공간에 종료 문자 포함.
- memcpy/memmove: memcpy는 영역 겹침 허용 안 함. memmove는 영역 겹침 처리.
- 직렬화: 데이터를 전송·저장 형식으로 변환. 필드 길이·순서·Endianness 명시.
- 구조체 전송: 메모리 통째 전송 시 Padding·정렬·Endianness·자료형 크기 문제 발생.

빌드·바이너리
- 전처리: 헤더 포함·매크로 확장·조건부 컴파일.
- 컴파일: 소스 코드를 대상 아키텍처 명령으로 변환.
- 어셈블: 어셈블리 코드를 목적 파일로 변환.
- 링크: 목적 파일·라이브러리 결합. 심볼 해석·주소 배치.
- 헤더: 선언·자료형·공유 인터페이스 제공. Include Guard로 중복 포함 방지.
- 심볼: 함수·전역 객체 등의 이름·주소·속성 정보.
- 정적 라이브러리: 필요한 목적 코드를 링크 결과에 포함.
- 동적 라이브러리: 실행 시 로더를 통한 라이브러리 연결·재배치.
- Cross Toolchain: 개발 호스트와 다른 대상 환경용 컴파일러·링커·라이브러리.
- Linker Script: 코드·데이터·스택 등의 메모리 배치 지정.
- ELF: 코드·데이터·심볼·로딩 정보 등을 담는 바이너리 형식.
- Map File: 섹션·심볼 주소·크기 확인. Flash·RAM 사용량 분석.
- objdump/readelf/nm: 명령어·ELF 구조·심볼 분석.
- 최적화: 코드 크기·속도·디버깅 정보 변화. Undefined Behavior 문제 노출 가능.

컴퓨터 구조
- Register: CPU 내부 연산·주소·상태 저장 공간.
- PC: 실행 명령 위치 관리.
- SP: 현재 스택 위치 관리.
- 함수 호출: ABI에 따라 인자·반환값 전달, 레지스터 보존, 스택 사용.
- Interrupt: 주변장치·타이머 등의 사건으로 실행 흐름 전환.
- Exception: Fault·시스템 호출 등 실행 중 사건 처리. 분류는 아키텍처 의존.
- Flash: 비휘발성 저장장치. 코드·설정 저장.
- SRAM: 리프레시 불필요 RAM. MCU 실행 데이터·스택 등에 사용.
- DRAM: 리프레시 필요 RAM. 대용량 주기억장치 등에 사용.
- Memory Map: 메모리·주변장치별 주소 범위.
- MMIO: 주소 읽기·쓰기를 통한 주변장치 레지스터 제어.
- Cache: 메모리 접근 지연 감소. CPU·DMA 간 데이터 일관성 관리 필요.
- DMA: CPU의 개별 데이터 복사 없이 주변장치·메모리 간 전송.
- Memory Barrier: 메모리 접근 순서 제약. 접근 자체의 원자성 보장 없음.

자료구조·알고리즘
- 배열: 연속 메모리·인덱스 접근 O(1). 중간 삽입·삭제 시 요소 이동.
- 연결 리스트: 노드 연결 구조. 노드 위치 확보 시 연결 변경 O(1), 탐색 O(n).
- Stack: LIFO. 함수 호출·역순 처리.
- Queue: FIFO. 작업·메시지 순서 처리.
- Ring Buffer: 고정 버퍼 순환 사용. Head/Tail·Full/Empty·덮어쓰기 정책 관리.
- Hash Table: 해시 기반 검색. 평균 O(1), 충돌·부하율에 따라 성능 변화.
- 이진 탐색: 정렬 데이터 검색 O(log n).
- 정렬: Quick 평균 O(n log n)·최악 O(n²). Merge O(n log n)·배열 구현 시 추가 버퍼 필요.
- 복잡도: 입력 크기에 따른 연산량·메모리 증가율.
- Producer–Consumer: 생산자 데이터 생성·소비자 처리. 버퍼·동기화·처리 속도 차이 관리.

동시성·설계
- Race Condition: 실행 순서에 따라 결과 변화.
- Critical Section: 공유 자원 접근 구간. 동시 진입 제한.
- Atomic: 연산의 불가분성 보장. 복합 처리 전체의 원자성은 별도 설계.
- Reentrancy: 실행 도중 재진입해도 상태 손상 없이 동작.
- Thread Safety: 여러 스레드의 동시 호출에도 동작 보장.
- FSM: 상태·이벤트·전이·동작으로 제어 흐름 표현.
- Event-Driven: 이벤트 발생에 따른 처리 실행.
- Blocking: 조건 충족·작업 완료까지 호출 흐름 대기.
- Non-blocking: 즉시 반환. 미완료 상태 별도 처리.
- 동기·비동기: 호출 흐름에서 완료 확인 또는 완료 통지·후속 조회.
- Timeout: 대기 시간 상한 설정. 시간 초과 후 취소·복구·재시도 정책 필요.
- 오류 복구: 오류 감지 → 원인 기록 → 재시도·재초기화·안전 상태 전환.

검증·디버깅
- Unit Test: 함수·모듈 단위 검증. 하드웨어 의존부 Mock/Stub 대체.
- Integration Test: 모듈·장치 간 인터페이스 검증.
- 경계값 검사: 길이 0·최대 길이·버퍼 경계·정수 범위 검사.
- Fault Injection: 통신 오류·할당 실패·전원 차단 등 오류 조건 주입.
- Static Analysis: 실행 없이 결함·규칙 위반 탐지.
- Sanitizer: 실행 중 메모리 오류·Undefined Behavior 등 탐지.
- Breakpoint: 지정 코드 위치에서 실행 정지.
- Watchpoint: 지정 메모리 접근·값 변경 시 실행 정지.
- Call Stack: 함수 호출 경로·복귀 위치 확인.
- Disassembly: 실제 명령어 확인. 최적화·레지스터 사용·Fault 위치 분석.
- Git: 변경 이력·차이·브랜치·병합 관리. 문제 발생 변경 추적.
- Make/CMake: 빌드 의존성·옵션·대상 관리. CMake는 빌드 시스템 생성.

MCU: 문서·전기·GPIO
- Datasheet: 핀·전압·전류·주파수·동작 조건 확인.
- Reference Manual: 주변장치 동작·레지스터 설정 확인.
- Errata: 실리콘 결함·제약·우회 방법 확인.
- 회로도: 전원·신호 연결·외부 소자·핀 기능 확인.
- 타이밍 다이어그램: 신호 순서·주기·Setup/Hold 조건 확인.
- Logic Level: 입력 High/Low 판정 전압·출력 전압 보장 조건.
- Pull-up/Pull-down: 미구동 입력의 기본 논리값 유지.
- Push-Pull: High·Low 모두 출력 구동.
- Open-Drain: Low 구동·출력 해제. High 형성용 Pull-up 필요.
- Alternate Function: GPIO 핀을 UART·SPI·Timer 등 주변장치에 연결.
- Debounce: 스위치 접점 튐 제거. 시간 필터·상태 확인 활용.

MCU: 레지스터·인터럽트
- Register-Level Programming: 레지스터 직접 접근을 통한 주변장치 설정·제어.
- Read-Modify-Write: 읽기 → 비트 변경 → 쓰기. 동시 접근·레지스터 부작용 주의.
- RO/RW/W1C: 읽기 전용·읽기/쓰기·1 기록 시 해당 비트 클리어.
- CMSIS: Cortex 코어 접근·장치 정의용 표준 인터페이스.
- NVIC: Cortex-M 인터럽트 활성화·우선순위·Pending 관리.
- Vector Table: 초기 스택 값·예외 처리 함수 주소 목록.
- Nested Interrupt: 우선순위에 따른 ISR 선점.
- ISR: 인터럽트 원인 확인·처리·플래그 해제. 장시간 처리는 Task 등으로 이관.
- Interrupt Latency: 인터럽트 발생부터 ISR 실행 시작까지 지연.
- 공유 데이터: ISR·메인 루프·Task 간 원자성·접근 순서·수명 보장.

MCU: Timer·주변장치
- Prescaler: 입력 클록 분주. Counter 증가 주파수 결정.
- Timer 주기: 일반적인 상향 카운터 기준 (PSC+1) × (ARR+1) / 타이머 클록.
- Input Capture: 입력 신호 발생 시 Counter 저장. 주기·펄스 폭 측정.
- Output Compare: Counter와 비교값 일치 시 이벤트·출력 생성.
- PWM: 주기·Duty로 펄스 출력 제어.
- Tick Wraparound: 카운터 최대값 후 순환. unsigned 차이 계산과 측정 가능 구간 관리.
- ADC: 아날로그 전압을 디지털 값으로 변환. 기준 전압·해상도·샘플링 시간 확인.
- DAC: 디지털 값을 아날로그 출력으로 변환.
- Watchdog: 제한 시간 내 갱신 없으면 Reset 등 복구 동작.
- Clock Tree/PLL: 클록 소스·분주·배수 설정. CPU·버스·주변장치 주파수 결정.

MCU: 부팅·저장·고장 분석
- Reset Handler: 초기화 코드 시작점. 실행 환경 구성 후 main() 호출.
- .data 초기화: 비휘발성 메모리의 초기값을 RAM 실행 위치로 복사.
- .bss 초기화: 해당 RAM 영역을 0으로 설정.
- Reset Cause: 전원·외부 Reset·Watchdog·Brownout 등 재시작 원인.
- Brownout: 공급 전압 저하 감지. 오동작 방지용 Reset 등 수행.
- Flash: Erase 단위·Program 단위·쓰기 횟수 제한 존재.
- 설정값 저장: CRC·버전·복사본·갱신 순서로 손상 검출·복구 설계.
- 저전력: 클록·전원 차단 범위와 Wake-up 지연·유지 데이터 확인.
- HardFault: 잘못된 접근·명령 등 Fault 처리. 원인 레지스터·예외 스택 분석.
- Stacked PC/LR: 예외 발생 지점·복귀 정보 추적. Fault 명령 주변 분석.
- JTAG/SWD: 디버거 연결·다운로드·실행 제어·메모리 접근.
- Oscilloscope: 전압·파형·노이즈·시간 특성 측정.
- Logic Analyzer: 디지털 신호 캡처·프로토콜 해석.

MCU: 통신
- UART: 비동기 직렬 통신. Baud Rate·Data Bit·Parity·Stop Bit 일치 필요.
- SPI: 클록 기반 직렬 통신. CS·CPOL·CPHA·비트 순서 설정.
- I2C: SDA/SCL 기반 주소 통신. Open-Drain·Pull-up·ACK/NACK 사용.
- Interrupt 송수신: 전송·수신 이벤트마다 ISR 처리. CPU 부하·지연 고려.
- DMA 송수신: 버퍼 단위 전송. 완료 시점·버퍼 수명·재사용 시점 관리.
- Framing: 바이트 흐름에서 메시지 경계 식별. 길이·구분자·헤더 활용.
- Parser: 부분 수신·잘못된 길이·손상 프레임 처리. 오류 후 경계 재탐색.
- CRC: 비트 오류 검출. 송수신 측 Polynomial·초기값·반전 조건 일치 필요.
- Retry: 일시 오류 재시도. 횟수·간격·중복 명령 처리 정책 필요.

RTOS
- Task 상태: Running·Ready·Blocked 등. 대기 조건 충족 시 Ready 전환.
- Context Switching: 현재 Task 실행 문맥 저장·다음 Task 문맥 복원.
- Preemptive Scheduling: 우선순위 등 조건에 따라 실행 Task 선점.
- Cooperative Scheduling: Task의 양보·Blocking 등에서 실행권 전환.
- Task Priority/Interrupt Priority: 스케줄러 우선순위·하드웨어 예외 우선순위. 별도 체계.
- Mutex: 소유권 기반 상호 배제. 소유 Task의 해제 원칙.
- Semaphore: 자원 개수·이벤트 통지 관리. 일반적으로 Mutex 같은 소유권 없음.
- Message Queue: 데이터 전달·대기·Task 간 실행 분리.
- Event Flag: 비트 조합을 통한 이벤트 통지·조건 대기.
- Deadlock: 상호 자원 대기로 진행 중단. 잠금 순서·보유 시간 관리.
- Starvation: 실행권·자원 획득이 계속 지연되는 상태.
- Priority Inversion: 낮은 우선순위 Task의 자원 보유로 높은 우선순위 Task 지연.
- Priority Inheritance: 자원 보유 Task 우선순위 임시 상승으로 역전 지연 완화.
- ISR API: ISR 전용 API·호출 가능 우선순위 확인. 일반적인 Blocking 대기 금지.
- 주기 실행: 이전 실행 완료 기준 지연은 Drift 누적 가능. 기준 시각 기반 실행 활용.
- Deadline: 작업 완료 시한. 평균 속도보다 시한 충족 여부 중요.
- Latency/Jitter/WCET: 응답 지연·시간 변동·최악 실행 시간.
- Stack 측정: 최대 사용량·여유 공간 확인. 예외 경로·인터럽트 중첩 포함.

Embedded Linux / BSP
- Kernel/User Space: 권한·주소 공간 분리. 시스템 호출로 커널 기능 요청.
- Process/Thread: 프로세스별 주소 공간 분리. 동일 프로세스 스레드는 주소 공간 공유.
- IPC: Pipe·Socket·공유 메모리 등 프로세스 간 통신.
- Signal: 프로세스·스레드 대상 비동기 사건 통지. Handler 내 호출 가능 함수 제한.
- 파일 디스크립터: 열린 파일·장치·Socket 등을 가리키는 프로세스 내 정수 식별자.
- open/read/write/close: 대상 열기·데이터 읽기·쓰기·참조 해제.
- ioctl/poll/mmap: 장치별 제어·준비 상태 대기·주소 공간 매핑.
- 부분 읽기·쓰기: 요청 길이보다 짧은 처리 허용. 남은 길이·종료·오류 처리.
- errno: 실패 원인 확인. 반환값 실패 판정 후 확인.
- Virtual Memory/MMU: 가상 주소를 물리 주소로 변환. 접근 권한·주소 공간 격리.
- Page Table: 가상 페이지별 매핑·접근 속성 저장.
- 부팅: Boot ROM → Bootloader → Kernel → Init. 플랫폼별 단계 차이.
- U-Boot: 하드웨어 초기화·부팅 이미지 적재·커널 실행.
- Device Tree: 하드웨어 구성 기술. 주소·IRQ·클록·연결 관계 전달.
- Root Filesystem: 실행 파일·라이브러리·설정·장치 접근 환경 제공.
- Init: 사용자 공간 초기 프로세스. 서비스 시작·프로세스 관리.
- Kconfig/Kbuild: 커널 구성 선택·소스 빌드 규칙.
- Yocto/Buildroot: Toolchain·커널·패키지·Root Filesystem 기반 시스템 이미지 구성.
- dmesg/journalctl: 커널 로그·서비스 로그 확인.
- strace: 시스템 호출·반환값·오류 추적.
- Core Dump: 비정상 종료 시 메모리·실행 상태 저장. 사후 분석.

Linux Device Driver
- Device Model: Bus·Device·Driver 관계를 통한 장치 발견·연결·수명 관리.
- Device Matching: Device Tree Compatible·장치 ID 등으로 Driver 연결.
- probe: Driver 연결 시 자원 확보·하드웨어 초기화·인터페이스 등록.
- 해제 경로: 진행 작업 중단·IRQ/DMA 정리·인터페이스 해제·자원 반환.
- Character Device: 파일 연산 기반 장치 인터페이스.
- Device File: /dev 장치 노드. Major/Minor 번호로 장치 식별.
- file_operations: 사용자 공간 파일 연산과 Driver 함수 연결.
- copy_to_user/copy_from_user: 사용자 공간·커널 공간 간 데이터 복사. 미복사 바이트 수 확인.
- ioremap: MMIO 물리 주소 범위를 커널 접근용 주소로 매핑.
- readl/writel: MMIO 접근 함수. 일반 포인터 역참조 대신 플랫폼 접근 규칙 적용.
- IRQ Handler: 인터럽트 문맥 처리. Sleep 불가.
- Threaded IRQ: 인터럽트 작업을 커널 스레드 문맥에서 처리. Sleep 가능한 처리 활용.
- Process Context: Task 실행 문맥. 잠금·실행 상태 등 조건 충족 시 Sleep 가능.
- Wait Queue: 조건 충족까지 Task 대기. 조건 변경 후 Wake-up.
- Workqueue: 작업을 Worker Thread에서 지연 처리.
- Mutex/Spinlock: Mutex는 대기 시 Sleep 가능. Spinlock은 바쁜 대기·Sleep 금지.
- kmalloc/vmalloc: 물리적으로 연속된 메모리·가상 주소상 연속된 메모리 할당.
- GFP Flag: 할당 시 Sleep·회수 등 허용 동작 지정. 실행 문맥에 맞춰 선택.
- devm_*: 장치 수명에 연결된 자원 자동 해제. 비동기 작업 중단은 별도 설계.
- DMA Address: 장치가 사용하는 주소. CPU 가상·물리 주소와 구분.
- DMA Mapping: 버퍼의 장치 접근 준비·주소 획득·전송 후 해제.
- Buffer Ownership: CPU·장치의 접근 권한·시점 관리.
- Cache Coherency: CPU·장치가 최신 데이터를 관찰하도록 일관성 유지.
- Memory Ordering: 데이터·상태·장치 통지 순서 보장. Coherency와 별개.

C++
- 참조: 기존 객체의 별칭. 대상 객체 수명 확인.
- 클래스: 데이터·동작·접근 제어 묶음.
- 생성자·소멸자: 객체 초기화·수명 종료 처리.
- 상속·다형성: 공통 인터페이스를 통한 구현별 동작 호출.
- 가상 함수: 실행 시 실제 객체 타입에 따른 함수 선택. 간접 호출·객체 크기 비용 고려.
- 가상 소멸자: 기반 클래스 포인터를 통한 파생 객체 삭제 시 필요.
- RAII: 객체 수명과 자원 획득·반환 연결. 오류 경로의 자원 누수 방지.
- unique_ptr: 단독 소유권. 복사 불가·이동 가능.
- shared_ptr: 참조 횟수 기반 공동 소유권. 순환 참조·관리 비용 주의.
- 복사·이동: 값·자원 복제 또는 자원 이전. 이동도 구현에 따라 비용 발생.
- Rule of Zero/Five: 자원 관리 타입 활용 또는 복사·이동·소멸 연산 일관 설계.
- 템플릿: 타입·값에 따른 코드 생성. 추상화·재사용, 코드 크기 증가 고려.
- STL: 컨테이너별 메모리 배치·재할당·반복자 무효화·복잡도 확인.
- 예외·RTTI: 런타임 기능·지원 코드 비용 확인. Toolchain·프로젝트 정책에 맞춰 사용.

차량 전장
- CAN: ID 기반 메시지 통신. 비파괴 중재·오류 검출·Bus-off 처리.
- CAN-FD: CAN 대비 데이터 영역 확대·데이터 구간 고속 전송.
- UDS: 진단 서비스 규약. 세션·보안 접근·고장 코드·재프로그래밍.
- AUTOSAR: 차량 소프트웨어 구조·인터페이스 표준.
- ISO 26262: 차량 전기·전자 시스템 기능 안전. 위험 분석·ASIL·검증 추적.

IoT·네트워크·USB
- BLE: 저전력 무선 통신. Advertising·Connection·GATT Service/Characteristic.
- Wi-Fi: 무선 LAN 연결. 연결 상태·인증·재접속 관리.
- IP: 주소 지정·패킷 전달.
- TCP: 연결 기반 바이트 스트림. 순서·재전송 제공. 메시지 경계 별도 설계.
- UDP: 데이터그램 통신. 전달·순서·중복 제거 보장 없음.
- Socket: 네트워크 통신 인터페이스. 연결·송수신·Timeout·종료 관리.
- MQTT: Broker 기반 Publish/Subscribe. Topic·QoS·연결 상태 관리.
- TLS: 통신 암호화·무결성·상대 인증. 인증서·키 관리 필요.
- Ethernet MAC/PHY: 프레임 처리·매체 접근 담당 / 전기 신호 송수신 담당.
- USB Descriptor: 장치·인터페이스·Endpoint 정보 제공.
- USB Enumeration: Host의 장치 식별·주소 할당·구성 선택.
- USB Endpoint: 장치 내 논리적 데이터 송수신 지점.
- USB Transfer: Control·Bulk·Interrupt·Isochronous. 신뢰성·대역폭·주기 특성 구분.

FPGA/SoC·로보틱스·산업제어
- AXI4-Lite: 제어 레지스터 접근용 메모리 매핑 인터페이스.
- AXI4-Stream: 주소 없는 연속 데이터 전달. Valid/Ready 흐름 제어.
- PS/PL: 프로세서 시스템·프로그래머블 로직 구분.
- BRAM: FPGA 내부 메모리 자원.
- SoC DMA: 메모리·PL 간 데이터 이동. 버퍼·Cache·완료 시점 관리.
- Encoder: 위치·회전량·속도 측정. 분해능·방향·카운터 처리.
- Motor Control: PWM·전류·속도·위치 피드백 제어.
- PID: 현재 오차·누적 오차·오차 변화율 기반 제어. 주기·포화·적분 누적 관리.
- ROS 2: Node·Topic·Service·Action 기반 로봇 소프트웨어 통신.
- Real-Time Ethernet: 전송 주기·동기화·최악 지연 관리.
- 고장 복구: 오류 감지·진단 기록·출력 제한·안전 상태·재시작 정책.

보안·업데이트
- Secure Boot: 신뢰 기준에서 시작해 실행 이미지 서명 검증.
- Firmware Update/OTA: 이미지 수신·검증·설치·부팅 확인.
- Rollback: 업데이트 실패 시 이전 정상 이미지 복구.
- Anti-Rollback: 취약한 구버전 설치 방지. 버전·보안 카운터 관리.
- 전원 차단 대응: 설치 도중 중단돼도 부팅·복구 가능하도록 이미지·메타데이터 갱신 설계.


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
