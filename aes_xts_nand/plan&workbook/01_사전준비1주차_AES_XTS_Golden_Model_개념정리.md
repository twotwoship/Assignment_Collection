# 사전준비 1주차 — AES-XTS Golden Model 학습·검증 계획

> 목표: FPGA를 만들기 전에 **“정답이 무엇인지 판단할 수 있는 환경”**을 먼저 만든다.

이 문서는 AES/AES-XTS를 어느 수준까지 공부해야 하는지, 어떤 공식 문서를 봐야 하는지, 왜 OpenSSL EVP 기반 Golden Model을 먼저 만들어야 하는지를 정리한 문서다.

---

# 1. 1주차 최종 목표

7일 뒤 다음 구조가 있으면 된다.

```text
aes_xts_golden/
│
├── src/
│   └── xts_golden.c
│
├── vectors/
│   └── NIST test vectors
│
├── input/
│   ├── plaintext_512.bin
│   └── plaintext_4096.bin
│
├── output/
│   ├── cipher_lba0.bin
│   ├── cipher_lba1.bin
│   └── decrypted.bin
│
├── scripts/
│   ├── lba_to_tweak.py
│   └── run_test.sh
│
└── README.md
```

최종적으로:

```bash
./scripts/run_test.sh
```

한 번으로 최소 다음 결과가 나와야 한다.

```text
[PASS] NIST XTS-AES-128 known-answer test
[PASS] 512-byte encrypt/decrypt round trip
[PASS] LBA/tweak change produces different ciphertext
[PASS] 4096-byte encrypt/decrypt round trip
[PASS] ALL TESTS
```

---

# 2. 왜 FPGA보다 Golden Model을 먼저 만드는가

나중에 FPGA AES-XTS RTL을 구현했을 때 다음 결과만 보면 된다.

```text
동일한 Key
동일한 Tweak/LBA
동일한 Plaintext
        │
        ├──────── OpenSSL Golden Model
        │               ↓
        │          Ciphertext A
        │
        └──────── FPGA AES-XTS
                        ↓
                   Ciphertext B

A == B ?
```

`A == B`이면 FPGA 결과가 소프트웨어 기준 모델과 bit-exact하게 일치한다.

이 구조가 없으면 FPGA 결과가 틀렸을 때 다음 중 무엇이 문제인지 판단하기 어렵다.

```text
AES Core 문제
XTS Tweak 문제
Key1/Key2 순서 문제
Endian 문제
LBA 변환 문제
AXI/DMA byte order 문제
```

---

# 3. 반드시 볼 공식 문서

## 3.1 NIST FIPS 197 — AES

공식 페이지:

- https://csrc.nist.gov/pubs/fips/197/final

공식 PDF:

- https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf

2023년 갱신판은 문서 표현과 구조를 정리했지만 AES 알고리즘 자체에는 기술적 변경이 없다.

### 이번 주에 읽을 부분

- Section 1 — Introduction
- Section 2.1 — Terms and Acronyms
- Section 5 — Algorithm Specifications
- Appendix B — Cipher Example은 선택

### 페이지 기준

2023 PDF 기준:

- PDF 뷰어 9/46: 본문 p.1, Section 1
- PDF 뷰어 10/46: 본문 p.2, Definitions 시작
- PDF 뷰어 19/46: 본문 p.11, Section 5 / Table 3
- Appendix B는 본문 p.34 부근

### 반드시 이해할 것

```text
AES는 Block Cipher이다.

AES Block Size = 128 bit = 16 byte
```

AES-128 / AES-192 / AES-256의 숫자는 **block size가 아니라 key size**이다.

| 종류 | Key | Block | Round |
|---|---:|---:|---:|
| AES-128 | 128 bit | 128 bit | 10 |
| AES-192 | 192 bit | 128 bit | 12 |
| AES-256 | 256 bit | 128 bit | 14 |

---

# 4. AES 내부는 어디까지 공부해야 하는가

현재 단계에서는 다음 흐름을 이해하면 충분하다.

```text
Plaintext
   ↓
AddRoundKey
   ↓
SubBytes
   ↓
ShiftRows
   ↓
MixColumns
   ↓
AddRoundKey
   ↓
반복
   ↓
Ciphertext
```

알아야 하는 용어:

- Plaintext
- Ciphertext
- Key
- Block
- Round
- SubBytes
- ShiftRows
- MixColumns
- AddRoundKey
- Key Expansion

이번 주에는 다음을 깊게 파지 않는다.

- S-Box 생성 수학
- GF(2^8) 증명
- AES 공격기법
- 하드웨어 파이프라인 최적화

이 내용은 FPGA AES RTL 구현 단계에서 다시 공부한다.

---

# 5. NIST SP 800-38E — XTS-AES

공식 페이지:

- https://csrc.nist.gov/pubs/sp/800/38/e/final

공식 PDF:

- https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38e.pdf

NIST는 XTS-AES를 저장장치의 고정 길이 data unit 보호를 위한 AES 운용모드로 승인한다.

XTS-AES는:

```text
Confidentiality 제공
Authentication 제공하지 않음
```

즉 저장된 내용을 알아보기 어렵게 만들지만, 암호문이 변조되지 않았음을 자체적으로 인증하는 모드는 아니다.

---

# 6. SP 800-38E에서 읽을 부분

이번 주에는 사실상 본문 전체가 짧다.

특히:

- Section 3 — Introduction
- Section 4 — Conformance
- Section 5 — Ciphertext Stealing ordering은 훑어보기

PDF 기준:

- PDF 뷰어 9/12: 본문 p.1, Introduction
- PDF 뷰어 10/12: 본문 p.2, Conformance
- PDF 뷰어 11/12: 본문 p.3, CTS

이번 프로젝트 초기 버전에서는 512 B와 4096 B처럼 16-byte 배수만 처리하므로 CTS를 우선 제외해도 된다.

---

# 7. Data Unit의 의미

SSD 예:

```text
LBA 0
[              512 byte              ]

LBA 1
[              512 byte              ]

LBA 2
[              512 byte              ]
```

512-byte sector 하나를 하나의 XTS Data Unit으로 정할 수 있다.

```text
Data Unit #0 = 512 B
Data Unit #1 = 512 B
Data Unit #2 = 512 B
```

중요:

```text
AES Block != XTS Data Unit
```

AES block은 항상:

```text
16 byte
```

512 B Data Unit이라면:

```text
512 / 16 = 32
```

즉 32개의 AES block이 들어간다.

```text
512-byte Data Unit

┌────16B────┬────16B────┬────16B────┬ ... ┬────16B────┐
│ Block 0   │ Block 1   │ Block 2   │     │ Block 31  │
└───────────┴───────────┴───────────┴─────┴───────────┘
```

4096 B를 하나의 Data Unit으로 정의하면:

```text
4096 / 16 = 256 AES blocks
```

이다.

---

# 8. 왜 XTS가 필요한가

단순히 동일 AES key로 동일 plaintext block을 독립적으로 암호화하면 위치가 달라도 동일한 암호문이 나오는 문제가 생길 수 있다.

XTS에서는 위치와 연관된 tweak를 사용한다.

개념적으로:

```text
저장 데이터
+
저장 위치를 나타내는 Tweak
+
Key
        ↓
      XTS
        ↓
Ciphertext
```

따라서:

```text
Plaintext 동일
Key 동일
LBA/Tweak 다름
```

이면 일반적으로:

```text
Ciphertext 다름
```

이어야 한다.

---

# 9. Key1 / Key2

XTS는 AES key를 두 개 사용한다.

개념적으로:

```text
Key1
→ 실제 Data 암/복호화

Key2
→ 초기 Tweak 생성
```

## XTS-AES-128

```text
Key1 = 128 bit
Key2 = 128 bit

Total XTS Key = 256 bit = 32 byte
```

## XTS-AES-256

```text
Key1 = 256 bit
Key2 = 256 bit

Total XTS Key = 512 bit = 64 byte
```

따라서:

```text
AES-256-XTS
```

라는 이름을 보고 전체 key가 256 bit라고 생각하면 안 된다.

OpenSSL도 XTS-AES-128에 256-bit 전체 key, XTS-AES-256에 512-bit 전체 key를 요구한다.

---

# 10. 첫 구현은 XTS-AES-128

초기 FPGA 학습/검증에서는:

```text
XTS-AES-128
```

부터 시작하는 것이 좋다.

즉:

```text
Key1 = 16 B
Key2 = 16 B
Total = 32 B
```

이다.

AES-128이 AES-256보다 round 수와 key schedule이 단순하기 때문에 첫 RTL 구현에 적합하다.

---

# 11. NIST XTSVS

공식 XTS Validation System 문서:

- https://csrc.nist.gov/csrc/media/projects/cryptographic-algorithm-validation-program/documents/aes/xtsvs.pdf

CAVP Block Cipher Modes 페이지:

- https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/CAVP-TESTING-BLOCK-CIPHER-MODES

이 페이지에서 공식 XTS-AES Test Vectors ZIP을 받을 수 있다.

### 볼 부분

XTSVS:

- Section 2 — Scope
- Section 5 — Design Philosophy
- Section 6.1 — Configuration Information
- Section 6.2 — The XTSGenAES Test

XTSVS는:

```text
Key
Tweak 또는 DataUnitSeqNumber
Plaintext/Ciphertext
```

를 입력하여 구현 결과와 기대 결과를 비교한다.

즉 우리 프로젝트도 같은 철학으로 검증하면 된다.

---

# 12. Encrypt → Decrypt만 성공하면 충분하지 않은 이유

다음 테스트:

```text
Plaintext
↓
내 Encrypt
↓
Ciphertext
↓
내 Decrypt
↓
Plaintext
```

에서 원본이 복구됐다고 하자.

이것만으로는 표준 XTS 구현임을 보장할 수 없다.

왜냐하면 Encrypt와 Decrypt가 **동일한 방식으로 잘못 구현**되어도 서로 상쇄되어 원본이 복구될 수 있기 때문이다.

따라서 반드시:

```text
NIST Expected Ciphertext
```

와 비교하는 Known Answer Test가 필요하다.

---

# 13. 진짜 Golden Model 검증

```text
NIST Key
NIST Tweak
NIST Plaintext
      │
      ↓
OpenSSL EVP AES-XTS
      │
      ↓
Generated Ciphertext
      │
      │ cmp
      ↓
NIST Expected Ciphertext
```

결과:

```text
MATCH
```

가 되어야 한다.

그 이후에야 OpenSSL 프로그램을 FPGA의 기준 모델로 사용할 수 있다.

---

# 14. 왜 `openssl enc`보다 EVP API를 쓰는가

AES-ECB 같은 단순 AES 확인에는 `openssl enc` CLI를 사용해도 된다.

하지만 AES-XTS Golden Model은 C 코드에서 OpenSSL EVP API를 직접 사용하는 쪽으로 간다.

권장 구조:

```text
내 C 프로그램
    ↓
OpenSSL EVP API
    ↓
AES-128-XTS
```

주요 API:

```c
EVP_CIPHER_fetch()
EVP_CIPHER_CTX_new()
EVP_CipherInit_ex2()
EVP_CipherUpdate()
EVP_CipherFinal_ex()
EVP_CIPHER_CTX_free()
EVP_CIPHER_free()
```

OpenSSL 공식 문서:

- AES/XTS 설명:
  https://docs.openssl.org/3.5/man3/EVP_aes_128_gcm/
- EVP Cipher API:
  https://docs.openssl.org/3.5/man3/EVP_EncryptInit/
- `openssl enc`:
  https://docs.openssl.org/3.5/man1/openssl-enc/

---

# 15. OpenSSL XTS에서 중요한 제약

OpenSSL의 XTS 구현은 streaming을 지원하지 않는다.

따라서 하나의 512 B Data Unit을:

```text
16 B
16 B
16 B
...
```

로 나눠서 여러 번:

```c
EVP_CipherUpdate()
EVP_CipherUpdate()
EVP_CipherUpdate()
```

호출하는 식으로 처리하면 안 된다.

하나의 Data Unit은 한 번의 Update로 처리한다.

```c
EVP_CipherUpdate(
    ctx,
    output,
    &out_len,
    input,
    512
);
```

4096 B를 하나의 Data Unit으로 정했다면 4096 B 전체를 한 번의 Update로 처리한다.

반대로:

```text
8개의 512 B sector
```

를 처리하는 것이라면 각 sector마다 별도 XTS operation과 별도 tweak/LBA를 사용한다.

이 두 경우는 암호문이 동일하지 않다.

---

# 16. LBA와 Tweak

OpenSSL은 `LBA`라는 개념을 직접 알지 못한다.

OpenSSL에 전달하는 것은:

```text
16-byte tweak input
```

이다.

프로젝트에서는:

```text
LBA
 ↓
128-bit little-endian Tweak Input
 ↓
OpenSSL AES-XTS
```

형태로 만든다.

예:

```text
LBA = 0
Tweak = 00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
```

```text
LBA = 1
Tweak = 01 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
```

IEEE XTS 계열의 data-unit tweak integer는 AES 입력 전에 little-endian byte array로 변환한다.

이 byte ordering은 나중에 RTL, DMA, Linux Driver까지 그대로 영향을 주므로 반드시 NIST vector와 맞춰 고정해야 한다.

---

# 17. 첫 실습 데이터

처음에는 random보다 규칙적인 데이터가 좋다.

512 B:

```text
00 01 02 03 ... FE FF
00 01 02 03 ... FE FF
```

형태.

Python:

```python
from pathlib import Path

Path("plaintext_512.bin").write_bytes(
    bytes(i & 0xff for i in range(512))
)
```

이 패턴은 나중에 FPGA waveform이나 DMA dump를 볼 때 byte ordering 문제를 찾기 쉽다.

---

# 18. Key도 처음에는 고정

XTS-AES-128 예:

```text
Key1:
00 01 02 03 04 05 06 07
08 09 0A 0B 0C 0D 0E 0F

Key2:
10 11 12 13 14 15 16 17
18 19 1A 1B 1C 1D 1E 1F
```

전체:

```text
000102030405060708090a0b0c0d0e0f
101112131415161718191a1b1c1d1e1f
```

32 byte이다.

---

# 19. 필수 테스트 1 — NIST Known Answer Test

```text
NIST Key
NIST Tweak
NIST Plaintext
        ↓
OpenSSL EVP
        ↓
Generated CT
        ↓
NIST Expected CT와 비교
```

반드시:

```text
MATCH
```

여야 한다.

---

# 20. 필수 테스트 2 — 512 B Round Trip

```text
plaintext_512.bin
        ↓
AES-XTS Encrypt
        ↓
cipher_512.bin
        ↓
AES-XTS Decrypt
        ↓
decrypted_512.bin
```

```bash
cmp plaintext_512.bin decrypted_512.bin
```

성공해야 한다.

---

# 21. 필수 테스트 3 — 다른 LBA/Tweak

```text
Same Plaintext
Same Key
LBA 0
  ↓
cipher_lba0.bin
```

```text
Same Plaintext
Same Key
LBA 1
  ↓
cipher_lba1.bin
```

두 암호문은 달라야 한다.

```bash
cmp cipher_lba0.bin cipher_lba1.bin
```

`cmp`가 다르다고 판단하는 것이 정상이다.

---

# 22. 필수 테스트 4 — 4096 B

4096 B를 하나의 Data Unit으로 정의한 테스트:

```text
4096 B
= 256 AES blocks
```

Encrypt → Decrypt → `cmp`를 수행한다.

주의:

```text
하나의 4096 B Data Unit
```

과:

```text
8개의 512 B Data Unit
```

은 같은 것이 아니다.

최종 SSD 프로젝트에서는 어떤 단위를 XTS Data Unit으로 할 것인지 명시적으로 결정해야 한다.

---

# 23. 이번 주에는 CTS를 우선 제외

512 B:

```text
512 % 16 = 0
```

4096 B:

```text
4096 % 16 = 0
```

둘 다 complete block으로만 구성된다.

따라서 초기 FPGA 구현은:

```text
Ciphertext Stealing 제외
```

로 가져갈 수 있다.

SP 800-38E도 complete-block data unit만 지원하는 구현에서는 CTS 구성요소가 불필요할 수 있음을 설명한다.

---

# 24. 7일 전체 계획

| Day | 공부 | 실습 | 통과조건 |
|---|---|---|---|
| 1 | FIPS 197 / AES | AES-128 단일 block 검증 | FIPS AES vector MATCH |
| 2 | SP 800-38E / XTS | Data Unit, Tweak, Key1/2 정리 | XTS 구조 설명 가능 |
| 3 | OpenSSL EVP | `xts_golden.c` 작성 | NIST XTS vector 1개 MATCH |
| 4 | 파일 암복호화 | 512 B round-trip | `cmp` PASS |
| 5 | NIST XTSVS | 공식 vector 구조 분석 | Known Answer Test PASS |
| 6 | LBA/Tweak + 4 KiB | LBA 0/1, 512/4096 테스트 | 모두 PASS |
| 7 | 자동화 | `run_test.sh` | 한 명령으로 ALL PASS |

---

# 25. 1주차 종료 구두시험

## Q1. AES block size는?

```text
128 bit = 16 byte
```

## Q2. AES-128의 128은?

```text
Key length
```

## Q3. XTS-AES-128 전체 key 길이는?

```text
256 bit = 32 byte
```

## Q4. 왜 key가 두 개인가?

```text
Key1 → data 암/복호화
Key2 → tweak 생성
```

## Q5. Data Unit은?

```text
XTS가 하나의 위치 단위로 처리하는 저장 데이터 영역
예: 512 B sector
```

## Q6. LBA 역할은?

```text
저장 위치를 식별하고
그 위치에 대응하는 tweak 입력값을 만드는 기준
```

## Q7. 같은 plaintext + key, 다른 LBA라면?

```text
Ciphertext가 달라져야 한다.
```

## Q8. Golden Model이 필요한 이유는?

```text
FPGA 결과가 표준 AES-XTS 결과와
bit-exact하게 일치하는지 판단하기 위해서
```

---

# 26. 최종 통과조건

단순 round-trip만으로 끝내지 않는다.

```text
① NIST Known Answer Test
OpenSSL output == NIST Expected
PASS

② 512 B Round Trip
Plaintext == Decrypted
PASS

③ LBA/Tweak 차이
Same PT + Same Key + LBA0
!=
Same PT + Same Key + LBA1
PASS

④ 4096 B Round Trip
PASS

⑤ 모든 테스트 자동화
./scripts/run_test.sh
→ ALL PASS
```

이 상태가 되면 다음 FPGA 단계에서 같은 vector를 사용하여:

```text
OpenSSL Golden Model
vs
FPGA AES-XTS RTL
```

을 직접 비교할 수 있다.

---

# 27. 이번 주에 공부하지 않아도 되는 것

다음은 후순위다.

- GF(2^8) 수학적 증명
- S-Box 생성 원리
- AES 암호해독 공격
- XTS security proof
- Ciphertext Stealing RTL
- OpenSSL 내부 소스코드 분석
- AES pipeline 최적화

이번 주 목표는 오직:

```text
표준 이해
↓
NIST Vector 이해
↓
OpenSSL EVP Golden Model
↓
자동 PASS/FAIL
```

이다.

---

# 28. 참고 문서

1. NIST FIPS 197 — Advanced Encryption Standard  
   https://csrc.nist.gov/pubs/fips/197/final

2. NIST FIPS 197 PDF  
   https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf

3. NIST SP 800-38E — XTS-AES  
   https://csrc.nist.gov/pubs/sp/800/38/e/final

4. NIST SP 800-38E PDF  
   https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38e.pdf

5. NIST XTS-AES Validation System (XTSVS)  
   https://csrc.nist.gov/csrc/media/projects/cryptographic-algorithm-validation-program/documents/aes/xtsvs.pdf

6. NIST CAVP Block Cipher Modes / XTS Test Vectors  
   https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/CAVP-TESTING-BLOCK-CIPHER-MODES

7. OpenSSL AES/XTS EVP documentation  
   https://docs.openssl.org/3.5/man3/EVP_aes_128_gcm/

8. OpenSSL EVP Cipher routines  
   https://docs.openssl.org/3.5/man3/EVP_EncryptInit/

9. OpenSSL `enc` command  
   https://docs.openssl.org/3.5/man1/openssl-enc/

10. Intel Cryptography Primitives — XTS tweak little-endian 설명 참고  
    https://www.intel.com/content/www/us/en/docs/crypto-primitives-library/developer-guide-reference/2025-2/aesencryptxts-direct-aesdecryptxts-direct.html

---

작성 기준일: 2026-08-24
