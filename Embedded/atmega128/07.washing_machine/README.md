# ATmega128A 기반 세탁기 제어 시스템

세탁·헹굼·탈수 과정을 상태에 따라 제어하고,
버튼과 UART 명령으로 조작하는 MCU 프로젝트 
발표자료 첨부 : 가. _LeeYB_Washing_machine_ppt.pptx

**주요 기술:** ATmega128A · C · FSM · Timer Interrupt · PWM · UART

## 시연 영상

[세탁기 동작 영상 보기](https://www.youtube.com/shorts/kGAorJIMjIk)

## 주요 구현

- 세탁·헹굼·탈수 상태 전환
- 버튼을 이용한 운전 시간 설정 및 시작·정지
- PWM 기반 DC 모터 속도·방향 제어
- UART 명령을 통한 시작·정지 제어
- FND를 이용한 남은 시간 표시
- 타이머 인터럽트를 이용한 시간 관리

## 소스 구성

| 파일 | 주요 내용 |
|---|---|
| [main.c](./main.c) | 초기화 및 타이머 인터럽트 |
| [pwm.c](./pwm.c) | PWM 및 모터 제어 관련 구현 |
| [uart0.c](./uart0.c) | UART 송수신 및 명령 처리 |
| [button.c](./button.c) | 버튼 입력 처리 |
| [FND.c](./FND.c) | FND 표시 |
| [led.c](./led.c) | LED 제어 |

## 구현 과정에서 확인한 한계

- 상태 함수 내부의 반복문으로 공통 이벤트 처리 확장이 어려움
- 상태 제어와 표시·입력·모터 처리 사이의 결합도가 높음
- 잘못된 명령과 예외 상황에 대한 처리 보완 필요
