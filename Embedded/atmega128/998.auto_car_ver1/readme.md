# ATmega128A 장애물 회피·Bluetooth 제어 차량

> 초음파 센서 기반 장애물 회피 및 Bluetooth 수동 제어
> 
> 보고서 첨부

## 1. 프로젝트 개요

| 항목 | 내용 |
|---|---|
| 개발 목적 | 센서·통신·모터를 연동한 차량 제어 |
| MCU | ATmega128A |
| 개발 언어 | C |
| 주요 기술 | FSM, UART, PWM, Timer, 외부 인터럽트 |
| 입력 장치 | 초음파 센서 3개, Bluetooth, 버튼 |
| 출력 장치 | DC 모터, FND, LED |

## 2. 시연 자료

- [차량 주행 시연](https://www.youtube.com/shorts/NwpaM5rJNO4)

## 3. 주요 구현

| 구분 | 구현 내용 |
|---|---|
| 수동 주행 | Bluetooth 명령 기반 전진·후진·좌회전·우회전·정지 |
| 자동 주행 | 좌·중앙·우 거리 비교 기반 장애물 회피 |
| 모터 제어 | PWM 기반 속도·방향 제어 |
| 상태 관리 | 수동·자동·주행 기록 확인 모드 |
| 통신 | UART 수신 인터럽트 기반 명령 수신 |
| 표시 | FND 기반 주행 정보 표시 |

## 4. 비차단 처리 개선

| 항목 | 기존 방식 | 개선 방식 |
|---|---|---|
| Trigger 생성 | `_delay_us()` 기반 대기 | 타이머 카운터 기반 경과 시간 확인 |
| 처리 구조 | HIGH·대기·LOW 순차 실행 | HIGH 설정·시간 확인·LOW 전환 분리 |
| Echo 감지 | 외부 인터럽트 | 상승·하강 플래그 활용 |
| 개선 범위 | 기존 센서 처리 | 왼쪽 센서 Trigger 처리 우선 적용 |

**처리 순서**

1. Trigger HIGH 및 시작 시각 기록
2. 경과 시간 확인 후 Trigger LOW
3. 외부 인터럽트로 Echo 상승·하강 감지
4. 완료 플래그 확인 및 다음 측정 준비

## 5. 현재 상태

- 왼쪽 센서 비차단 Trigger 처리 코드 작성
- main 루프에서 해당 처리 함수 실행
- 주행 FSM 호출부 주석 처리
- 전체 주행 제어와의 통합·동작 검증 필요

## 6. 소스 구성

| 파일 | 역할 |
|---|---|
| [main.c](./main.c) | 초기화·실험 루프 |
| [menu.c](./menu.c) | 주행 모드·상태 관리 |
| [motor.c](./motor.c) | 모터 구동·속도 제어 |
| [ultrasonic.c](./ultrasonic.c) | 초음파 Trigger·측정 처리 |
| [interrupt.c](./interrupt.c) | 타이머·외부 인터럽트 |
| [uart1.c](./uart1.c) | Bluetooth 통신 |
| [uart0.c](./uart0.c) | PC UART 통신 |
| [fnd.c](./fnd.c) | 주행 정보 표시 |
| [button.c](./button.c) | 버튼 입력 |
