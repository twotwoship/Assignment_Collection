```mermaid
flowchart TB

    %% ================================================================
    %% 0. 전체 범례
    %% ================================================================

    subgraph LEGEND["범례"]
        direction LR
        LG1["실선 화살표<br/>실제 실행·호출·데이터 흐름"]
        LG2["점선 화살표<br/>선택 설치·조건 분기"]
        LG3["원통형 노드<br/>파일·DB·설정이 실제로 저장되는 위치"]
    end

    %% ================================================================
    %% 1. 소스 준비
    %% ================================================================

    subgraph PREPARE["① 소스 준비 — 설치 스크립트를 실행하기 전에 사람이 직접 해야 하는 단계"]
        direction TB

        P1["Git 저장소 Clone 또는 소스 복사"]
        P2["raspberry_pi/s_w/slotguard/로 이동"]
        P3["python3 -m venv .venv"]
        P4["source .venv/bin/activate"]
        P5["python -m pip install -r requirements.txt"]
        P6["설치되는 Python 패키지<br/>Flask 3.1.3<br/>pyserial 3.5<br/>python-docx 1.2.0"]
        P7["주의<br/>install_slotguard.sh는 .venv를 만들지 않음<br/>.venv/bin/python이 없으면 즉시 종료"]

        P1 --> P2 --> P3 --> P4 --> P5 --> P6 --> P7
    end

    %% ================================================================
    %% 2. 설치 단계
    %% ================================================================

    subgraph INSTALL["② Raspberry Pi 최초 설치 — system/ 아래 설치 스크립트"]
        direction TB

        INSTALL_START{"어떤 장치 기능을 설치하는가?"}

        P7 --> INSTALL_START

        %% ------------------------------------------------------------
        %% 2-A. 핵심 서비스 설치
        %% ------------------------------------------------------------

        subgraph CORE_INSTALL["A. install_slotguard.sh 사용자명 — 핵심 서비스·키오스크 설치"]
            direction TB

            C1["sudo ./system/install_slotguard.sh 사용자명"]
            C2{"실행 사용자와<br/>.venv/bin/python 존재 확인"}
            C3["필요 Debian 패키지 확인 및 설치<br/>xserver-xorg-video-fbdev<br/>xserver-xorg-input-evdev<br/>fonts-noto-cjk"]
            C4["사용자를 하드웨어 접근 그룹에 추가<br/>dialout · video · input"]

            C5["slotguard.service.in의 변수 치환<br/>사용자 · 그룹 · 프로젝트 절대경로"]
            C6[("생성<br/>/etc/systemd/system/slotguard.service")]
            C7["서비스 실행 명령<br/>.venv/bin/python app.py<br/>WorkingDirectory = slotguard/<br/>실패 시 3초 후 자동 재시작"]

            C8["전원 제어 도우미 설치"]
            C9[("복사<br/>/usr/local/sbin/slotguard-power")]
            C10[("권한<br/>/etc/sudoers.d/slotguard-power")]
            C11["Flask 실행 사용자는 비밀번호 없이<br/>poweroff 또는 reboot만 실행 가능"]

            C12["LCD X 세션 실행 파일 설치"]
            C13[("복사<br/>/usr/local/bin/slotguard-xsession")]
            C14[("복사<br/>/usr/share/xsessions/slotguard.desktop")]

            C15["Chromium 정책 설치"]
            C16[("복사<br/>/etc/chromium/policies/managed/<br/>slotguard-kiosk.json")]
            C17["번역 · 브라우저 로그인 · 비밀번호 저장 비활성화"]

            C18["LCD Xorg 설정 설치"]
            C19[("복사<br/>/etc/X11/xorg.conf.d/<br/>90-slotguard-lcd.conf")]
            C20["fbdev 화면 + ADS7846 터치 보정값 설정"]

            C21["WirePlumber 음량 정책 설치"]
            C22[("복사<br/>사용자 홈/.config/wireplumber/<br/>wireplumber.conf.d/90-slotguard-volume.conf")]
            C23["ALSA Headphones PCM을<br/>100% · Unmute로 설정 및 저장"]

            C24["LightDM 자동 로그인 설정"]
            C25[("생성<br/>/etc/lightdm/lightdm.conf.d/<br/>90-slotguard.conf")]
            C26["autologin-user = 실행 사용자<br/>autologin-session = slotguard<br/>user-session = slotguard"]
            C27["기존 /etc/lightdm/lightdm.conf가 있으면<br/>원본 백업 후 session 값을 slotguard로 변경"]

            C28["systemctl daemon-reload"]
            C29["systemctl enable slotguard.service"]
            C30["주의<br/>enable만 수행<br/>현재 즉시 start하지는 않음"]

            C31["install_time_helper.sh 사용자명<br/>자동 호출"]

            C1 --> C2
            C2 -->|정상| C3 --> C4
            C2 -->|.venv 또는 사용자 없음| C2_FAIL["오류 종료"]

            C4 --> C5 --> C6 --> C7
            C7 --> C8 --> C9 --> C10 --> C11
            C11 --> C12 --> C13 --> C14
            C14 --> C15 --> C16 --> C17
            C17 --> C18 --> C19 --> C20
            C20 --> C21 --> C22 --> C23
            C23 --> C24 --> C25 --> C26 --> C27
            C27 --> C28 --> C29 --> C30 --> C31
        end

        %% ------------------------------------------------------------
        %% 2-B. 시간 도우미
        %% ------------------------------------------------------------

        subgraph TIME_INSTALL["B. install_time_helper.sh — 메인 설치에서 자동 호출"]
            direction TB

            T1["실행 사용자 검증"]
            T2[("복사<br/>/usr/local/sbin/slotguard-set-time")]
            T3[("생성<br/>/etc/sudoers.d/slotguard-set-time")]
            T4["Flask 사용자가 비밀번호 없이<br/>검증된 Unix Timestamp로 시스템 시간 변경 가능"]
            T5["timedatectl set-timezone Asia/Seoul"]
            T6["timedatectl set-ntp false"]
            T7["외부 인터넷/NTP 없이<br/>관리자 스마트폰 시간 사용"]

            C31 --> T1 --> T2 --> T3 --> T4 --> T5 --> T6 --> T7
        end

        %% ------------------------------------------------------------
        %% 2-C. 네트워크 전환 도우미
        %% ------------------------------------------------------------

        subgraph NETWORK_HELPER_INSTALL["C. install_network_helper.sh — 운영 AP와 개발 Wi-Fi 전환 기능"]
            direction TB

            N0["선행 조건<br/>NetworkManager에 개발 Wi-Fi 프로필이 이미 존재해야 함"]
            N1["sudo ./system/install_network_helper.sh<br/>사용자 개발-WiFi-프로필 wlan0"]
            N2["사용자 · Wi-Fi 인터페이스 · 개발 프로필 검증"]

            N3[("복사<br/>/usr/local/sbin/slotguard-network")]
            N4[("생성<br/>/etc/slotguard/network.conf")]
            N5["network.conf 내용<br/>interface = wlan0<br/>operation = slotguard-ap<br/>development = 기존 개발 Wi-Fi"]

            N6[("생성<br/>/etc/sudoers.d/slotguard-network")]
            N7["Flask 사용자가 비밀번호 없이 실행 가능<br/>slotguard-network status<br/>slotguard-network operation<br/>slotguard-network development"]
            N8["설치 시점에는<br/>현재 Wi-Fi 연결을 변경하지 않음"]

            N0 --> N1 --> N2 --> N3 --> N4 --> N5 --> N6 --> N7 --> N8
        end

        %% ------------------------------------------------------------
        %% 2-D. 로컬 AP 생성
        %% ------------------------------------------------------------

        subgraph AP_INSTALL["D. install_slotguard_ap.sh — 장치 전용 로컬 Wi-Fi 생성"]
            direction TB

            A1["선행 조건<br/>install_network_helper.sh가 먼저 실행되어<br/>/etc/slotguard/network.conf가 있어야 함"]
            A2["sudo ./system/install_slotguard_ap.sh wlan0"]
            A3{"slotguard-ap 프로필이<br/>이미 존재하는가?"}
            A4["사용자에게 AP 비밀번호 입력 요청<br/>8~63자 · 입력 화면 숨김"]
            A5["nmcli connection add<br/>type wifi · mode ap<br/>SSID = SLOT-GUARD"]
            A6["NetworkManager AP 설정<br/>2.4GHz · Channel 6<br/>WPA-PSK<br/>IPv4 Shared"]
            A7[("NetworkManager 프로필<br/>connection = slotguard-ap<br/>IP = 192.168.4.1/24")]
            A8["slotguard-ap 자동 연결 ON<br/>우선순위 100"]
            A9["개발 Wi-Fi 자동 연결 OFF"]
            A10["다음 부팅부터 기본 운영 모드<br/>http://192.168.4.1:5000"]
            A_FAIL["이미 존재하면 중복 생성 없이 오류 종료"]

            A1 --> A2 --> A3
            A3 -->|아니오| A4 --> A5 --> A6 --> A7 --> A8 --> A9 --> A10
            A3 -->|예| A_FAIL
        end

        N8 --> A1

        %% ------------------------------------------------------------
        %% 2-E. LCD 드라이버
        %% ------------------------------------------------------------

        subgraph LCD_INSTALL["E. install_waveshare4_lcd.sh — Waveshare LCD 부팅 설정"]
            direction TB

            L1["sudo ./system/install_waveshare4_lcd.sh"]
            L2{"파일 존재 확인<br/>/boot/firmware/config.txt"}
            L3[("백업 디렉터리 생성<br/>/boot/firmware/<br/>slotguard-backup-before-lcd")]
            L4["기존 config.txt와 overlay가 있으면<br/>최초 한 번만 백업"]
            L5["고정된 Git Commit에서<br/>waveshare35a-overlay.dtb 다운로드"]
            L6["SHA-256 검증<br/>변조·잘못된 파일이면 설치 중단"]
            L7[("설치<br/>/boot/firmware/overlays/<br/>waveshare35a.dtbo")]
            L8["config.txt의 기존 SLOT-GUARD 설정 제거"]
            L9["새 부팅 설정 추가<br/>dtparam=spi=on<br/>enable_uart=1<br/>dtoverlay=waveshare35a,rotate=90"]
            L10["재부팅 후 기대 장치<br/>Framebuffer + ADS7846 Touchscreen<br/>UART 활성화"]
            L_FAIL["config.txt 없음 또는<br/>다운로드·해시 실패 시 오류 종료"]

            L1 --> L2
            L2 -->|있음| L3 --> L4 --> L5 --> L6
            L6 -->|정상| L7 --> L8 --> L9 --> L10
            L2 -->|없음| L_FAIL
            L6 -->|불일치| L_FAIL
        end

        INSTALL_START -->|항상: 장치 서비스| C1
        INSTALL_START -.->|전용 AP를 사용할 때| N0
        INSTALL_START -.->|Waveshare LCD 최초 설치 시| L1

        INSTALL_DONE["설치 완료<br/>서비스는 Enable 상태<br/>AP·LCD 설정은 다음 부팅부터 적용"]

        T7 --> INSTALL_DONE
        A10 --> INSTALL_DONE
        L10 --> INSTALL_DONE
    end

    %% ================================================================
    %% 3. 전원 인가와 Linux 부팅
    %% ================================================================

    subgraph BOOT["③ 전원 인가·재부팅 — Linux와 ATmega128A가 동시에 시작"]
        direction TB

        B1["Raspberry Pi + ATmega128A 전원 ON"]
        B2["Raspberry Pi Bootloader가<br/>/boot/firmware/config.txt 적용"]
        B3["SPI ON · UART ON<br/>Waveshare Overlay · 화면 회전 적용"]
        B4["Raspberry Pi OS 부팅"]
        B5["NetworkManager 시작"]
        B6["slotguard-ap 자동 연결"]
        B7["SSID: SLOT-GUARD<br/>Pi 주소: 192.168.4.1"]
        B8["systemd multi-user.target"]
        B9["LightDM 시작"]

        INSTALL_DONE --> B1
        B1 --> B2 --> B3 --> B4
        B4 --> B5 --> B6 --> B7
        B4 --> B8
        B4 --> B9

        %% ATmega boot
        subgraph AT_BOOT["ATmega128A 전원 인가"]
            direction TB

            ATB1["atmega128/FINAL/<br/>ATMEGA128A_INTEGRATION/main.c"]
            ATB2["Sys_Init 실행"]
            ATB3["Systick_Init<br/>1ms Timer Tick"]
            ATB4["Uart_Init<br/>USART0 9600 bps · RX/TX Ring Buffer"]
            ATB5["Sensors_Init<br/>IR ×2 · SG255 Home ×2"]
            ATB6["Stepper_Init<br/>X/Y 28BYJ-48"]
            ATB7["Servo_Init<br/>MG996R · 50Hz PWM"]
            ATB8["Fsm_Init<br/>상태 Context 초기화"]
            ATB9["sei<br/>인터럽트 전체 활성화"]
            ATB10["STATE_RECOVERY_REQUIRED"]
            ATB11["Homing_Start<br/>X/Y축 동시 원점 탐색"]
            ATB12{"X/Y 원점 센서<br/>모두 감지?"}
            ATB13["현재 위치를 0,0으로 설정"]
            ATB14["STATE_IDLE<br/>새 명령 수신 준비"]
            ATB15["스텝 한도 초과<br/>코일 해제 · 수동 원점 대기"]

            B1 --> ATB1 --> ATB2 --> ATB3 --> ATB4 --> ATB5 --> ATB6
            ATB6 --> ATB7 --> ATB8 --> ATB9 --> ATB10 --> ATB11 --> ATB12
            ATB12 -->|예| ATB13 --> ATB14
            ATB12 -->|아니오| ATB15
            ATB15 -->|사람이 원점 이동 후 센서 감지| ATB13
        end
    end

    %% ================================================================
    %% 4. systemd가 Python 서비스 실행
    %% ================================================================

    subgraph APP_START["④ slotguard.service와 app.py 시작"]
        direction TB

        S1["systemd가 Enable된<br/>slotguard.service 실행"]
        S2["실행 사용자 = 설치 시 지정 사용자<br/>WorkingDirectory = slotguard/"]
        S3["ExecStart<br/>.venv/bin/python app.py"]

        S4["app.py 모듈 Import"]
        S5["database.py Import"]
        S6["system_time_service.py Import"]
        S7["uart_service.py Import"]

        S8["load_auth_config"]
        S9{"slotguard_auth.json<br/>존재?"}
        S10["기존 Secret Key·관리자 계정 읽기"]
        S11["임시 Secret Key 생성<br/>관리자 미설정 상태"]

        S12["Flask app 생성<br/>Session 보안·30분 Idle Timeout 설정"]
        S13["database.init_db 실행"]
        S14[("slotguard.db<br/>테이블 생성 또는 기존 DB Migration")]
        S15["일정 · 장치 상태 · 좌표 · 설정<br/>System Time · Reset 상태 준비"]

        S16["VoiceAlertManager 생성"]
        S17["UartService 생성<br/>음성 Callback + 시간 준비 Callback 연결"]

        S18["if __name__ == main"]
        S19["uart_service.start"]
        S20["slotguard-uart Daemon Thread 생성"]
        S21["Flask app.run<br/>0.0.0.0:5000 · debug=False"]

        S22{"프로세스 종료·예외?"}
        S23["systemd가 3초 대기 후<br/>app.py 자동 재시작"]

        B8 --> S1 --> S2 --> S3 --> S4
        S4 --> S5 --> S13 --> S14 --> S15
        S4 --> S6
        S4 --> S7
        S4 --> S8 --> S9
        S9 -->|예| S10 --> S12
        S9 -->|아니오| S11 --> S12
        S12 --> S16 --> S17 --> S18
        S15 --> S18
        S18 --> S19 --> S20
        S18 --> S21
        S21 --> S22
        S22 -->|예| S23 --> S3
    end

    %% ================================================================
    %% 5. LCD Kiosk 실행
    %% ================================================================

    subgraph KIOSK_START["⑤ LightDM 자동 로그인과 Chromium LCD 실행"]
        direction TB

        K1["LightDM 설정 읽기<br/>autologin-user = 장치 사용자<br/>session = slotguard"]
        K2["slotguard.desktop 실행"]
        K3["/usr/local/bin/slotguard-xsession 실행"]
        K4["xset<br/>화면 보호기 · DPMS · 화면 Blank 해제"]
        K5["WirePlumber·ALSA 음량 설정<br/>실패해도 Kiosk 실행은 계속"]
        K6["Openbox Window Manager 실행"]
        K7["curl로 1초마다 확인<br/>http://127.0.0.1:5000/api/display-status"]
        K8{"Flask API가<br/>정상 응답?"}
        K9["계속 1초 대기"]
        K10["Chromium Kiosk 실행"]
        K11["--disable-gpu<br/>--disable-extensions<br/>--disable-translate<br/>--no-first-run"]
        K12["http://127.0.0.1:5000/display"]
        K13["templates/display.html"]
        K14["static/display.css + display.js"]
        K15["display.js가 1초마다<br/>/api/display-status Polling"]
        K16["LCD에 현재 시간·다음 복약·상태·버튼 표시"]

        B9 --> K1 --> K2 --> K3 --> K4 --> K5 --> K6 --> K7 --> K8
        K8 -->|아니오| K9 --> K7
        K8 -->|예| K10 --> K11 --> K12 --> K13 --> K14 --> K15 --> K16

        S21 -->|API 준비 완료| K8
    end

    %% ================================================================
    %% 6. UART Thread 상시 동작
    %% ================================================================

    subgraph UART_RUNTIME["⑥ uart_service.py — 백그라운드 UART Thread 상시 루프"]
        direction TB

        U1["slotguard-uart Thread 시작"]
        U2{"pyserial 사용 가능?"}
        U3["상태 = PYSERIAL_MISSING"]
        U4["/dev/serial0 Open 시도<br/>9600 bps · 8N1 · Timeout 0"]
        U5{"Serial 연결 성공?"}
        U6["상태 = CONNECTED"]
        U7["연결 실패 상태 저장<br/>5초 후 재연결"]
        U8["반복 Loop"]

        U9["ATmega 수신 Byte 읽기"]
        U10["LF 단위로 Frame 분리"]
        U11["ACK · WAIT · RESULT · ERROR 처리"]
        U12["DB에 ACK 시각·상태·오류 저장"]

        U13["System Reset 요청 확인"]
        U14["Home Return 요청 확인"]
        U15["TIMEOUT 대기 요청 확인"]
        U16["현재 Active Schedule 확인"]
        U17["새로 시간이 된 Schedule 확인"]

        U18["ACK가 없는 전송은<br/>10초마다 동일 Request ID·Payload 재전송"]
        U19["통신 끊김 감지 시 Serial Close<br/>5초 후 다시 연결"]

        S20 --> U1 --> U2
        U2 -->|아니오| U3
        U2 -->|예| U4 --> U5
        U5 -->|예| U6 --> U8
        U5 -->|아니오| U7 --> U4

        U8 --> U9 --> U10 --> U11 --> U12
        U12 --> U13 --> U14 --> U15 --> U16 --> U17 --> U18 --> U8
        U8 -->|Serial 예외| U19 --> U7
    end

    %% ================================================================
    %% 7. 사용자 접속과 일반 기능
    %% ================================================================

    subgraph USER_RUNTIME["⑦ 사용자 접속 — Flask Web과 장치 LCD가 동시에 사용"]
        direction TB

        PHONE1["관리자 스마트폰"]
        PHONE2["Wi-Fi SSID SLOT-GUARD에 접속"]
        PHONE3["http://192.168.4.1:5000 접속"]
        PHONE4["Flask app.py"]

        AUTH1{"관리자 설정 파일<br/>slotguard_auth.json 존재?"}
        AUTH2["/setup<br/>최초 관리자 ID·비밀번호 설정"]
        AUTH3[("slotguard_auth.json 저장<br/>권한 0600")]
        AUTH4["/login<br/>비밀번호 Hash 검증"]
        AUTH5["Flask Session 생성<br/>30분 동안 활동 없으면 만료"]

        WEB1["/ 대시보드"]
        WEB2["/schedules<br/>일정 등록·조회"]
        WEB3["/schedules/id/delete<br/>개별 일정 삭제"]
        WEB4["/schedules/reset<br/>Pi·AT 전체 초기화 요청"]
        WEB5["/api/status<br/>Web·DB·UART·좌표·시간 상태"]

        PHONE1 --> PHONE2 --> PHONE3 --> PHONE4 --> AUTH1
        AUTH1 -->|미설정| AUTH2 --> AUTH3 --> AUTH4
        AUTH1 -->|설정됨| AUTH4
        AUTH4 --> AUTH5
        AUTH5 --> WEB1
        AUTH5 --> WEB2
        AUTH5 --> WEB3
        AUTH5 --> WEB4
        AUTH5 --> WEB5

        B7 --> PHONE2
        S21 --> PHONE4

        %% Time setup
        TIME1{"현재 Boot ID가<br/>DB에 시간 설정 완료로 기록됨?"}
        TIME2["LCD 화면 = TIME_REQUIRED<br/>UART Scheduler 중지"]
        TIME3["스마트폰이 현재 Timestamp와<br/>Timezone을 /api/system-time으로 전송"]
        TIME4["system_time_service.py<br/>2024~2099 범위 · Timezone 검증"]
        TIME5["sudo -n /usr/local/sbin/<br/>slotguard-set-time Timestamp"]
        TIME6["root 권한 clock_settime"]
        TIME7["현재 /proc/.../boot_id와<br/>설정 시각을 slotguard.db에 저장"]
        TIME8["현재 부팅 동안만<br/>복약 Scheduler 실행 허용"]

        PHONE4 --> TIME1
        TIME1 -->|아니오| TIME2 --> TIME3 --> TIME4 --> TIME5 --> TIME6 --> TIME7 --> TIME8
        TIME1 -->|예| TIME8

        %% LCD helpers
        LCD_ACTION["LCD 환경 설정<br/>127.0.0.1 요청만 허용"]
        LCD_SETTING["음성 반복·볼륨 설정<br/>slotguard.db 저장"]
        LCD_VOLUME["음량 시험<br/>cVLC로 MP3 1회 재생"]
        LCD_NETWORK["운영/개발 Network 전환"]
        LCD_NET_HELPER["sudo slotguard-network<br/>operation 또는 development"]
        LCD_NMCLI["nmcli connection up<br/>개발 Wi-Fi 실패 시 운영 AP 복구"]
        LCD_POWER["시스템 재시작·종료"]
        LCD_PWR_HELPER["sudo slotguard-power<br/>reboot 또는 poweroff"]
        LCD_BLISTER["블리스터 초기화<br/>Active Dose가 없을 때만 허용"]

        K16 --> LCD_ACTION
        LCD_ACTION --> LCD_SETTING
        LCD_ACTION --> LCD_VOLUME
        LCD_ACTION --> LCD_NETWORK --> LCD_NET_HELPER --> LCD_NMCLI
        LCD_ACTION --> LCD_POWER --> LCD_PWR_HELPER
        LCD_ACTION --> LCD_BLISTER
    end

    %% ================================================================
    %% 8. 복약 일정 등록
    %% ================================================================

    subgraph SCHEDULE_CREATE["⑧ 관리자 Web에서 복약 일정 등록"]
        direction TB

        SC1["관리자 입력<br/>약 이름 · 날짜 · 시각 · 허용시간"]
        SC2["app.py schedule_page"]
        SC3["입력값 검증<br/>약 이름 길이 · 시간 형식 · 허용시간"]
        SC4{"같은 복약 시각의<br/>일정이 이미 있는가?"}
        SC5["DuplicateScheduleError<br/>등록 거부"]
        SC6["database.create_schedule"]
        SC7[("slotguard.db<br/>status = SCHEDULED<br/>좌표는 아직 미배정")]
        SC8["Web과 LCD에서<br/>다음 복약 일정으로 표시"]

        WEB2 --> SC1 --> SC2 --> SC3 --> SC4
        SC4 -->|예| SC5
        SC4 -->|아니오| SC6 --> SC7 --> SC8
    end

    %% ================================================================
    %% 9. 예약 시간 도달
    %% ================================================================

    subgraph DOSE_START["⑨ 복약 시간 도달 — Raspberry Pi가 MOVE 준비"]
        direction TB

        D1["UART Thread가 현재 시각과<br/>SCHEDULED 일정을 주기적으로 비교"]
        D2{"현재 부팅에서<br/>시간 동기화 완료?"}
        D3["Scheduler 대기<br/>명령 전송하지 않음"]
        D4["database.get_due_schedule"]
        D5["다음 슬롯 좌표 배정"]

        D6["실제 좌표 진행 순서<br/>00 → 01 → 02 → 03 → 04<br/>→ 14 → 13 → 12 → 11 → 10"]
        D7["8자리 MOVE Request ID 생성"]
        D8[("DB 상태 변경<br/>SCHEDULED → MOVING<br/>좌표 · Request ID · 허용시간 저장")]
        D9["VoiceAlertManager.start"]
        D10["cVLC 실행<br/>audio/medicine_time.mp3<br/>설정 횟수만큼 반복"]
        D11["uart_service.build_move_command"]

        D12["Pi → ATmega<br/>MOVE · Request ID · X · Y · 남은 허용시간"]
        D13["DB에 MOVE 전송 시각 기록"]

        SC7 --> D1
        U17 --> D1
        D1 --> D2
        D2 -->|아니오| D3
        D2 -->|예| D4 --> D5 --> D6 --> D7 --> D8
        D8 --> D9 --> D10
        D8 --> D11 --> D12 --> D13
    end

    %% ================================================================
    %% 10. ATmega MOVE 처리
    %% ================================================================

    subgraph AT_MOVE["⑩ ATmega128A가 MOVE 명령 처리"]
        direction TB

        M1["USART0 RX Interrupt<br/>RX Ring Buffer에 Byte 저장"]
        M2["main.c Task_UartRx"]
        M3["uart.c Uart_ReadLine<br/>LF까지 한 Frame 추출"]
        M4["protocol.c Protocol_Parse"]
        M5{"Frame 형식·좌표·시간<br/>Request ID 유효?"}

        M6["ERROR · INVALID_FORMAT<br/>또는 상세 Error 전송"]
        M7{"동일 Request ID를<br/>이미 처리했는가?"}
        M8{"동일 ID + 동일 Payload?"}
        M9["기존 ACK/WAIT만 다시 전송<br/>모터는 절대 재구동하지 않음"]
        M10["ERROR · ID_CONFLICT<br/>물리 동작 차단"]

        M11{"FSM 상태가 IDLE인가?"}
        M12["ERROR · BUSY 또는<br/>RECOVERY_REQUIRED"]
        M13["Protocol_SendAck<br/>ACK · ID · MOVE"]
        M14["stepper.c<br/>Stepper_MoveToSlot X,Y"]
        M15["FSM<br/>STATE_IDLE → STATE_MOVING"]
        M16["Stepper_Task가 3ms마다<br/>X/Y 모터를 한 Step씩 진행"]
        M17{"목표 절대 Step에<br/>두 축 모두 도착?"}
        M18["현재 좌표 갱신"]
        M19["Protocol_SendWait<br/>WAIT · MOVE Request ID"]
        M20["FSM<br/>STATE_MOVING → STATE_AWAITING_DISPENSE"]

        D12 --> M1 --> M2 --> M3 --> M4 --> M5
        M5 -->|아니오| M6
        M5 -->|예| M7
        M7 -->|예| M8
        M8 -->|예| M9
        M8 -->|Payload 다름| M10
        M7 -->|신규 요청| M11
        M11 -->|아니오| M12
        M11 -->|예| M13 --> M14 --> M15 --> M16 --> M17
        M17 -->|아니오| M16
        M17 -->|예| M18 --> M19 --> M20
    end

    %% ================================================================
    %% 11. Pi가 ACK와 WAIT 처리
    %% ================================================================

    subgraph PI_READY["⑪ Raspberry Pi가 이동 완료를 DB와 LCD에 반영"]
        direction TB

        R1["Pi UART Thread가<br/>ACK · ID · MOVE 수신"]
        R2["Request ID 일치 확인"]
        R3[("DB move_ack_at 저장")]
        R4["WAIT · ID 수신"]
        R5["MOVE Request ID 일치 확인"]
        R6[("DB 상태 변경<br/>MOVING → READY_TO_DISPENSE<br/>ready_at 저장")]
        R7["LCD Polling이 새 상태 수신"]
        R8["화면 표시<br/>약 위치 이동 완료<br/>약 배출 버튼 활성화"]
        R9["음성 안내는 사용자가<br/>배출 버튼을 누를 때까지 계속"]

        M13 --> R1 --> R2 --> R3
        M19 --> R4 --> R5 --> R6 --> R7 --> R8 --> R9
    end

    %% ================================================================
    %% 12. 사용자 배출 승인
    %% ================================================================

    subgraph USER_APPROVAL["⑫ 사용자가 LCD에서 약 배출 승인"]
        direction TB

        DA1["사용자가 큰 약 배출 버튼 터치"]
        DA2["display.js<br/>POST /api/display/dispense"]
        DA3["display_local_only 검사<br/>127.0.0.1 또는 ::1만 허용"]
        DA4{"DB 상태가<br/>READY_TO_DISPENSE?"}
        DA5["요청 거부<br/>DISPENSE_NOT_ALLOWED"]
        DA6{"복약 허용시간이<br/>남아 있는가?"}
        DA7["database.prepare_dispense"]
        DA8["새 8자리 DISPENSE Request ID 생성"]
        DA9[("DB 상태 변경<br/>READY_TO_DISPENSE → DISPENSING")]
        DA10["VoiceAlertManager.stop<br/>복약 안내 음성 종료"]
        DA11["UART Thread가 DISPENSE Frame 생성"]
        DA12["Pi → ATmega<br/>DISPENSE · Request ID · X · Y"]

        R8 --> DA1 --> DA2 --> DA3 --> DA4
        DA4 -->|아니오| DA5
        DA4 -->|예| DA6
        DA6 -->|예| DA7 --> DA8 --> DA9 --> DA10 --> DA11 --> DA12
    end

    %% ================================================================
    %% 13. ATmega 배출 처리
    %% ================================================================

    subgraph AT_DISPENSE["⑬ ATmega128A 압출·센서 시퀀스"]
        direction TB

        E1["USART RX → Protocol_Parse"]
        E2{"Request ID·좌표 유효?<br/>현재 상태 AWAITING_DISPENSE?"}
        E3["ERROR 전송<br/>NOT_READY · COORD_MISMATCH 등"]
        E4{"동일 DISPENSE ID<br/>재수신?"}
        E5["기존 ACK만 재전송<br/>서보·모터 절대 재동작하지 않음"]
        E6["ACK · ID · DISPENSE"]
        E7["FSM<br/>STATE_AWAITING_DISPENSE → STATE_DISPENSING"]
        E8["Sensors_IrClear<br/>IR1·IR2 Interrupt 활성화"]
        E9["dispense.c Dispense_Start"]

        E10["1차 압출<br/>Y - Offset 이동<br/>Servo 180° → 50° → 0°<br/>즉시 180° 복귀"]
        E11{"전 과정에서<br/>IR 정제 감지?"}

        E12["2차 압출<br/>원래 Y 위치 이동<br/>Servo 압출·복귀"]
        E13{"IR 정제 감지?"}

        E14["3차 압출<br/>Y + Offset 이동<br/>Servo 압출 후 복귀와 동시에<br/>5초 감지 Window 시작"]
        E15{"5초 이내<br/>IR 정제 감지?"}

        E16["배출 성공<br/>DispenseResult = 1"]
        E17["미감지·빈 슬롯 추정<br/>DispenseResult = 2"]
        E18["IR Interrupt 비활성화<br/>서보 180° 복귀"]
        E19["RESULT · DISPENSE-ID · XY1 또는 XY2"]
        E20["FSM<br/>STATE_DISPENSING → STATE_AWAITING_RESULT_ACK"]
        E21["RESULT ACK가 없으면<br/>10초마다 RESULT 재전송<br/>최대 6회"]

        DA12 --> E1 --> E2
        E2 -->|아니오| E3
        E2 -->|예| E4
        E4 -->|예| E5
        E4 -->|신규| E6 --> E7 --> E8 --> E9 --> E10 --> E11
        E11 -->|예| E16
        E11 -->|아니오| E12 --> E13
        E13 -->|예| E16
        E13 -->|아니오| E14 --> E15
        E15 -->|예| E16
        E15 -->|아니오| E17
        E16 --> E18
        E17 --> E18
        E18 --> E19 --> E20 --> E21
    end

    %% ================================================================
    %% 14. Pi RESULT 처리
    %% ================================================================

    subgraph RESULT_PROCESS["⑭ Raspberry Pi가 RESULT를 확인하고 기록"]
        direction TB

        RP1["UART Thread가 RESULT 수신"]
        RP2{"DISPENSE Request ID와<br/>좌표가 일치?"}
        RP3["불일치 RESULT 무시<br/>현재 복약 상태 유지"]
        RP4["Pi → ATmega<br/>ACK · ID · RESULT"]
        RP5["ATmega FSM<br/>AWAITING_RESULT_ACK → IDLE"]

        RP6{"결과 R 값"}
        RP7["R = 1<br/>IR 정제 통과 성공"]
        RP8[("DB 상태<br/>DISPENSING → DISPENSED<br/>완료 시각 저장")]
        RP9["LCD<br/>정제 배출 확인 완료"]
        RP10["5초 후 HOME 화면"]
        RP11["다음 복약 일정 대기"]

        RP12["R = 2<br/>서보 동작 후 IR 미감지"]
        RP13[("DB 상태<br/>EMPTY_SLOT_CONFIRM")]
        RP14["empty_slot_confirm.mp3 재생"]
        RP15["LCD 질문<br/>약이 실제로 나왔습니까?"]
        RP16{"사용자 선택"}

        RP17["나왔음<br/>센서 미감지로 판단"]
        RP18[("수동 확인 완료<br/>복약 완료 기록")]

        RP19["안 나왔음<br/>현재 슬롯이 비었다고 판단"]
        RP20{"사용 가능한<br/>다음 좌표가 있는가?"}
        RP21["다음 슬롯 좌표 배정"]
        RP22["empty_slot_next.mp3 재생"]
        RP23["새 MOVE Request ID 생성"]
        RP24["다음 슬롯으로 MOVE<br/>⑨ 단계부터 다시 실행"]

        RP25["10개 슬롯 모두 사용 또는 빈 슬롯"]
        RP26[("blister_exhausted = 1")]
        RP27["blister_empty.mp3 재생"]
        RP28["LCD<br/>새 블리스터 교체 요청"]

        E19 --> RP1 --> RP2
        RP2 -->|아니오| RP3
        RP2 -->|예| RP4 --> RP5 --> RP6

        RP6 -->|1| RP7 --> RP8 --> RP9 --> RP10 --> RP11
        RP6 -->|2| RP12 --> RP13 --> RP14 --> RP15 --> RP16

        RP16 -->|나왔음| RP17 --> RP18 --> RP9
        RP16 -->|안 나왔음| RP19 --> RP20
        RP20 -->|예| RP21 --> RP22 --> RP23 --> RP24 --> D8
        RP20 -->|아니오| RP25 --> RP26 --> RP27 --> RP28
    end

    %% ================================================================
    %% 15. 시간 초과와 통신 오류
    %% ================================================================

    subgraph ERROR_FLOW["⑮ 시간 초과·통신 오류·잘못된 Frame 처리"]
        direction TB

        X1{"어느 단계에서<br/>허용시간이 만료되었는가?"}

        X2["READY_TO_DISPENSE에서 만료"]
        X3[("DB 상태 → MISSED<br/>error = DOSE_BUTTON_TIMEOUT")]
        X4["Pi → ATmega<br/>TIMEOUT · MOVE Request ID"]
        X5["ATmega 배출 대기 취소<br/>ACK · ID · TIMEOUT<br/>STATE_IDLE"]

        X6["MOVING에서 만료"]
        X7[("DB 상태 → COMM_ERROR<br/>MOVE_ACK_TIMEOUT 또는<br/>MOVE_READY_TIMEOUT")]

        X8["DISPENSING에서 만료"]
        X9[("DB 상태 → COMM_ERROR<br/>DISPENSE_ACK_TIMEOUT 또는<br/>RESULT_TIMEOUT")]

        X10["ACK 누락"]
        X11["Pi가 10초마다<br/>동일 Request ID·Payload 재전송"]
        X12["ATmega가 Duplicate ID 확인"]
        X13["ACK 또는 기존 응답만 재전송<br/>물리 모터·서보 재구동 없음"]

        X14["잘못된 UART Frame"]
        X15["ATmega → Pi<br/>ERROR · INVALID_FORMAT"]
        X16["Pi가 직전 Frame을 그대로 재전송"]
        X17["재전송할 Frame이 없으면<br/>오류 Log만 기록"]

        DA6 -->|아니오| X2
        U8 --> X1
        X1 -->|배출 버튼 대기| X2 --> X3 --> X4 --> X5
        X1 -->|이동 중| X6 --> X7
        X1 -->|배출 중| X8 --> X9

        U18 --> X10 --> X11 --> X12 --> X13
        M6 --> X14 --> X15 --> X16
        X16 --> X17
    end

    %% ================================================================
    %% 16. 원점 복귀와 블리스터
    %% ================================================================

    subgraph HOME_RETURN["⑯ 마지막 슬롯 처리와 0,0 원점 복귀"]
        direction TB

        H1["블리스터 마지막 좌표 처리 완료"]
        H2["Pi가 Home Return Request 생성"]
        H3[("DB에 home_request_id 저장")]
        H4["Pi → ATmega<br/>MOVE · Home-ID · 0 · 0 · 30초"]
        H5["ATmega ACK 후<br/>X/Y를 0,0으로 이동"]
        H6["ATmega → Pi<br/>WAIT · Home-ID"]
        H7["Pi → ATmega<br/>TIMEOUT · Home-ID"]
        H8["ATmega 배출 대기 상태 종료<br/>ACK · Home-ID · TIMEOUT"]
        H9[("DB Home Return 완료<br/>현재 좌표 = 0,0")]
        H10["LCD<br/>블리스터 교체 대기"]
        H11["사용자가 새 블리스터 장착"]
        H12["LCD 블리스터 초기화"]
        H13[("used_coordinates 초기화<br/>blister_exhausted = 0")]
        H14["다음 복약은 다시 00부터 시작"]

        RP25 --> H1 --> H2 --> H3 --> H4 --> H5 --> H6 --> H7 --> H8
        H8 --> H9 --> H10 --> H11 --> H12 --> H13 --> H14
    end

    %% ================================================================
    %% 17. 전체 Reset
    %% ================================================================

    subgraph RESET_FLOW["⑰ 관리자 전체 초기화 — Pi와 ATmega ACK가 맞아야 DB 삭제"]
        direction TB

        Z1["관리자 Web<br/>POST /schedules/reset"]
        Z2["database.prepare_system_reset"]
        Z3["8자리 RESET Request ID 생성"]
        Z4["음성 안내 중지"]
        Z5["UART Thread<br/>Pi → ATmega RESET · ID"]
        Z6["ATmega가 진행 중 모터·서보 중지"]
        Z7["ATmega 원점 복구 수행"]
        Z8["ATmega → Pi<br/>ACK · ID · RESET"]
        Z9{"RESET Request ID<br/>일치?"}
        Z10["일치하지 않는 ACK 무시<br/>DB 유지"]
        Z11["database.complete_system_reset"]
        Z12[("일정·상태·좌표를<br/>ACK 확인 후 초기화")]
        Z13["Pi UART Runtime 상태 초기화"]
        Z14["LCD HOME 또는 TIME_REQUIRED"]

        WEB4 --> Z1 --> Z2 --> Z3 --> Z4 --> Z5 --> Z6 --> Z7 --> Z8 --> Z9
        Z9 -->|아니오| Z10
        Z9 -->|예| Z11 --> Z12 --> Z13 --> Z14
    end

    %% ================================================================
    %% 18. 최종 시스템 상태
    %% ================================================================

    subgraph FINAL_STATE["⑱ 정상 운영 중 동시에 계속 실행되는 구성"]
        direction LR

        F1["Flask Main Thread<br/>Web·LCD HTTP 요청 처리"]
        F2["UART Daemon Thread<br/>일정 확인·Serial 송수신·재전송"]
        F3["SQLite<br/>일정·좌표·통신·결과·설정 영구 저장"]
        F4["Chromium Kiosk<br/>1초마다 상태 Polling"]
        F5["ATmega Superloop<br/>UART · FSM · Motor · Servo · Sensor Task"]
        F6["NetworkManager<br/>운영 AP 또는 개발 Wi-Fi"]
        F7["Voice Thread / cVLC<br/>필요할 때만 음성 재생"]
        F8["systemd<br/>app.py가 죽으면 3초 후 재실행"]

        S21 --> F1
        U8 --> F2
        S14 --> F3
        K15 --> F4
        ATB14 --> F5
        B5 --> F6
        D10 --> F7
        S22 --> F8

        F1 <--> F3
        F2 <--> F3
        F1 <--> F4
        F2 <--> F5
        F1 --> F7
        F1 --> F6
        F8 --> F1
        F8 --> F2
    end
```