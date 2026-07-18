# BBB 기준으로 작성하였음. 

### 준비물 및 단계 / U-boot 사용 기준
1. 우분투 환경 / 필자는 26.04 버전 / WSL2 사용
2. BBB용 크로스 컴파일러
3. 여러 패키지들 / 아래 정도 깔고 나머지는 그때 필요할때마다 설치해서 잘기억안남.
* autoconf automake bison bzip2 cmake flex g++ gawk gcc gettext git gperf help2man libncurses5-dev \
libstdc++6 libtool libtool-bin make patch python3-dev rsync texinfo unzip wget xz-utils
4. sd카드 2개의 파티션으로 만드셈 1번은 64MiB FAT32 포맷, 2번은 ext4 1기가짜리 
5. u-boot 2021.01 버전 / make 할때 크로스컴파일러로 하기 / MLO u-boot.img가 만들어질텐데 이 두갤 1번 sd카드 1번 파티션에 넣으셈.
    make ARCH=arm \     CROSS_COMPILE=arm-cortex_a8-linux-gnueabi- \     am335x_evm_defconfig
6. BBB는 시리얼 디버거를 기본적으로 제공해줘서 UART가 J1으로 보드상에 뽑혀져 있음. 대충 모듈 연결하면됨, 공홈에서 사라는거 사도 되고. 쿠팡에 8천원에 파는듯.
7. MobaXterm 사용했고 115200baud, 8 data bis, No parity, 1 stop bit, Flow control None
8. 터미널 연상태로 sd카드 넣고 S2 버튼 누른 상태로 전원인가하고 바로 아무키 갈기셈 바로 다음으로 넘어감
9. 짜잔 완료입니다. 이제 베어메탈을 만들던 RTOS를 만들던 기타등등 원하는데로 할수있다.

# 개념 설명
## 1단계 : ROM 코드
1. 칩 제조시 AM335x 내부 롬에 롬코드가 작성되어서 SPL(Secondary Program Loader, 내가 만들어 넣은 MLO)를 내부 SRAM(64KiB)에 로드함 롬 코드 단계 긑에서 SPL 코드의 시작으로 점프함.

## 2단계 : SPL 코드
1. 롬 코드에서 SPL로 점프 한뒤에 SRAM 내부에서 실행되는데 이때 TPL(내가 만든 U-boot.img)을 DRAM으로 로드함.
2. 반드시 DDR 초기화를 해줘야됨.

## 3단계 : TPL 코드
1. TPL 실행시 커널을 DRAM으로 로드한다. 원하는 경우 FDT나 초기 RAM disk를 추가 가능.
* 여기에서 DDR초기화를 반드시해줘야됨.

![ex_screenshot](./img/BBB_BOOT_LOAD.png)

# 작성
1. 1단계 : MLO DDR(512mb)을 초기화하고 같이 넣엇던 u-boot.img를 불러옴
2. 2단계 : u-boot.img 실행해서 명령어 창을 제공해줌 ^오^

### 고민사항
* 내가 직접 MLO를 만들어서 DDR초기화 시켜가지고 TPL을 실행시켜도 되고
* 내가 U-boot 딸깍 해서 U-boot 명령어 창에서 내가만든 rtos나 베어메탈을 해도되고 리눅스를 올려도되고 기타등등 해보자