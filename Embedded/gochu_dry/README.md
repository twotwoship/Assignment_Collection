# STM32 기반 고추건조기 온도 제어 시스템

> DS18B20 온도 측정과 릴레이 제어, 버튼 입력, FND·OLED 표시를 연동한 고추 건조기

## 프로젝트 기술서

| 항목 | 내용 |
|---|---|
| 작성자 | 이양배 |
| 프로젝트명 | STM32 기반 고추건조기 온도 제어 시스템 |
| 수행목표 | 설정 온도와 측정 온도를 비교하여 히터를 제어하고, 온도 및 동작 상태를 표시하는 시스템 구현 |
| 주요 구현 | DS18B20 센서 연동, 온도 설정 버튼 처리, 히터 릴레이 제어, FND·OLED 출력 |
| MCU | STM32F103C8T6 계열 |
| 사용 기술 | C / STM32 HAL / GPIO / EXTI / Timer / 1-Wire / I²C / UART |

## 1. 프로젝트 개요

- 제작 배경: 오제이 튜브 고추건조기 제작 콘텐츠를 참고한 C언어 및 STM32 주변장치 제어 학습
- 주요 기능: DS18B20 측정 온도와 사용자 설정 온도 비교를 통한 히터 릴레이 제어
- 표시 구성: FND 측정 온도 출력, OLED 설정 온도 및 릴레이 상태 출력
- 구현 목표: 센서 입력·제어 판단·구동부 출력·사용자 화면의 펌웨어 통합

## 2. 시스템 구성 및 동작 흐름

```text
[DS18B20 온도 센서] ── 1-Wire ────┐
[온도 증가·확정·감소 버튼] ─ EXTI ─┤
[시작 스위치] ──────── GPIO ──────┤
                                 ▼
                         [STM32F103 MCU]
                          │  │  │  │
             ┌────────────┘  │  │  └────────────┐
             ▼               ▼  ▼               ▼
       [히터 릴레이]       [FND] [I²C OLED]     [상태 LED]
        가열 ON/OFF       측정 온도  설정 온도      동작 상태
                                  릴레이 상태
```

1. GPIO·타이머·I²C·UART 주변장치 초기화
2. OLED 시작 화면 및 비트맵 애니메이션 출력
3. FND·DS18B20 초기화 및 TIM3 인터럽트 시작
4. 메인 루프의 버튼 이벤트 처리 및 온도 변환 요청
5. 온도 변환 완료 확인 및 측정값 읽기
6. 시작 스위치 상태·측정 온도에 따른 히터 제어
7. TIM3 인터럽트 기반 FND 갱신 및 상태 표시 처리

## 3. 세부 수행 내용

### 3.1 DS18B20 온도 측정 및 변환 상태 관리

- 1-Wire 기반 DS18B20 제어 및 12비트 해상도 설정
- 온도 변환 요청·완료 확인·측정값 읽기 함수 분리
- 메인 루프 내 변환 상태 확인 및 버튼 입력 처리
- 센서 읽기 함수의 Scratchpad CRC 검사

| 함수 | 역할 |
|---|---|
| `Ds18b20_Init_Simple()` | 센서 ROM 주소 지정 및 초기 설정 |
| `StartConverting()` | 온도 변환 명령 전송 |
| `cheakConverting()` | 변환 완료 여부 확인 및 상태 갱신 |
| `getTemper()` | 온도 데이터 읽기 |
| `getCurrentTemper()` | 저장된 측정 온도 반환 |

- 처리 방식: 변환 요청·완료 확인 분리
- 구현 제약: 메인 루프 및 센서 읽기 경로의 지연 함수 사용으로 완전한 비차단 처리에는 제한

### 3.2 외부 인터럽트 기반 온도 설정

- PB0·PB1·PB2 하강 에지 외부 인터럽트 설정
- 인터럽트의 버튼 이벤트 플래그 설정 및 메인 루프 `checkButton()`의 기능 실행
- `HAL_GetTick()` 기반 200ms 입력 간격 검사 및 반복 입력 억제
- 세 버튼의 입력 시간 변수 공유
- 설정 범위 0~99 및 범위 초과 시 순환 처리
- 확정 버튼 입력 시 제어 기준 온도 반영 및 OLED 점멸 시작

### 3.3 측정 온도 기반 히터 릴레이 제어

- `heaterControl()`: 현재 릴레이 상태·측정 온도 기반 ON/OFF 판단
- `heaterOnOff()`: 릴레이 GPIO 출력·내부 상태·LED·OLED 표시 갱신
- 시작 스위치 OFF 시 온도 측정 완료 후 히터 정지 처리

### 3.4 TIM3 인터럽트 기반 FND 표시

- 측정 온도의 10배 정수 변환 및 소수점 한 자리 표시
- `digit4_temp()` 기반 자릿수 순환 및 다중화 출력
- DIO·SCLK·RCLK GPIO 제어를 통한 세그먼트 데이터·자릿수 선택값 전송
- 센서 초기화 완료 및 1-Wire 라인 미사용 조건에서 TIM3 기반 표시 갱신

### 3.5 I²C OLED 및 상태 표시

- SSD1306 OLED 기반 설정 온도·릴레이 ON/OFF 상태 표시
- 시작 문구 및 비트맵 순차 출력
- 온도 설정 확정 시 OLED 점멸 표시
- LED 기반 시작 스위치·히터 상태 표시
- UART `printf()` 기반 버튼 이벤트 로그 출력

## 4. 주요 소스 구성

| 파일 | 역할 |
|---|---|
| [main.c](Core/Src/main.c) | 주변장치 초기화, 버튼 처리, 온도 측정 및 히터 제어 흐름 |
| [buttonmaster.c](Core/Src/buttonmaster.c) | 버튼 이벤트 처리 및 시작 스위치 상태 확인 |
| [heaterController.c](Core/Src/heaterController.c) | 설정 온도 관리 및 릴레이 제어 |
| [fnd_controller.c](Core/Src/fnd_controller.c) | GPIO 직렬 출력 및 FND 자릿수 표시 |
| [oledcontoroller.c](Core/Src/oledcontoroller.c) | OLED 화면 구성, 설정 온도·릴레이 상태 표시 |
| [stm32f1xx_it.c](Core/Src/stm32f1xx_it.c) | 버튼 EXTI 및 TIM3 인터럽트 처리 |
| [ds18b20.c](Core/Lib/Src/ds18b20.c) | 센서 초기화, 온도 변환 및 데이터 읽기 |
| [onewire.c](Core/Lib/Src/onewire.c) | 1-Wire 통신 처리 |
| [defines.h](Core/Inc/defines.h) | 기본 온도, 버튼 입력 간격, 제어 임계값 정의 |

## 5. 하드웨어 연결

| 기능 | STM32 핀 |
|---|---|
| DS18B20 Data | PA3 |
| 온도 증가 / 확정 / 감소 | PB0 / PB1 / PB2 |
| 시작 스위치 | PB12 |
| 히터 릴레이 | PB5 |
| 상태 LED 1 / 2 | PB6 / PB7 |
| FND RCLK / DIO / SCLK | PB13 / PB14 / PB15 |
| OLED I²C2 SCL / SDA | PB10 / PB11 |

## 6. 구현 및 학습 내용

- 센서 측정·제어 판단·화면 출력을 연결하는 MCU 펌웨어 구조 학습
- EXTI 이벤트 플래그 기반 버튼 처리 및 타이머 인터럽트 기반 FND 갱신 역할 분리
- 1-Wire·I²C 기반 주변장치 통합
- 온도 제어·버튼·표시 기능의 파일 단위 모듈화 및 호출 관계 구성


## 7. 참고 자료

- [오제이 튜브 고추건조기 제작 참고 영상](https://www.youtube.com/watch?v=YAkAKx72prs)

- 자료 구분: 제작 참고용 교육 콘텐츠, 본 프로젝트의 직접 시연 영상에 해당하지 않음

## 8. I²C 통신의 이론 및 프로젝트 적용

### 8.1 기본 개념

- I²C(Inter-Integrated Circuit): IC 간 데이터 교환을 위한 동기식 직렬 통신
- SDA: 주소·데이터 전달 신호선
- SCL: 데이터 전송 시점의 기준이 되는 클록 신호선
- Controller: 통신 시작·종료 및 클록 생성 담당
- Target: 주소에 따라 선택되어 송수신에 참여하는 장치
- 동일 버스에 여러 장치 연결 가능, 장치별 주소로 통신 대상 구분

### 8.2 전기적 특성

- 일반적인 오픈드레인 구성: LOW 직접 구동, HIGH는 출력 해제 후 풀업 저항으로 형성
- 통신 대기 상태: SDA·SCL 모두 HIGH
- 풀업 저항과 버스 정전용량에 따른 신호 상승시간 변화
- 풀업 저항 선정 시 상승시간과 장치의 LOW 구동 전류 고려

### 8.3 데이터 전송 절차

```text
START → 7비트 주소 + R/W → ACK → 8비트 데이터 → ACK → … → STOP
```

| 구분 | 동작 원리 |
|---|---|
| START | SCL HIGH 중 SDA의 HIGH → LOW 전환 |
| 주소·방향 | 7비트 대상 주소와 R/W 비트 전송, 0은 쓰기·1은 읽기 |
| 데이터 | 바이트 단위 전송, 최상위 비트(MSB)부터 송신 |
| ACK | 9번째 클록에서 수신 측이 SDA를 LOW로 구동 |
| NACK | 9번째 클록에서 SDA가 HIGH인 상태, 미응답 또는 읽기 종료 등에 사용 |
| STOP | SCL HIGH 중 SDA의 LOW → HIGH 전환 |

- 일반 데이터 전송: SCL LOW 구간에서 SDA 변경, HIGH 구간에서 값 유지
- START·STOP: 일반 데이터와 구분되는 SDA 전환 조건
- 읽기 종료: Controller가 마지막 바이트에 NACK 응답 후 STOP 생성

### 8.4 고추건조기 적용 내용

| 항목 | 프로젝트 적용 |
|---|---|
| Controller | STM32 |
| Target | SSD1306 OLED |
| 주변장치 | I²C2 |
| 신호 연결 | PB10: SCL / PB11: SDA |
| 클록 설정 | `hi2c2.Init.ClockSpeed = 400000` — Fast-mode의 최대 클록인 400kHz 설정 |
| 주소 모드 | `I2C_ADDRESSINGMODE_7BIT` |
| 활용 기능 | OLED 초기화 및 설정 온도·릴레이 상태·시작 화면 출력 |

```text
온도 설정·히터 상태 변경
        ↓
OLED 문자·화면 데이터 구성
        ↓
SSD1306_UpdateScreen()
        ↓
I²C2를 통한 OLED 화면 갱신
```

- 온도 측정 경로: DS18B20의 1-Wire 통신
- 화면 표시 경로: SSD1306의 I²C 통신
- 표시값 생성과 통신 출력을 분리한 주변장치 연동 구조

이론 참고: [NXP UM10204 — I²C-bus specification and user manual](https://www.nxp.com/docs/en/user-guide/UM10204.pdf)
