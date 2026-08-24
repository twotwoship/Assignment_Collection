# AES-XTS Golden Model — Day 1~7 실습 교안

> 대상: AES-XTS + Linux Driver + AXI DMA + FPGA 프로젝트를 준비하는 학습자  
> 목표: 7일 안에 **OpenSSL EVP 기반 AES-XTS Golden Model + NIST Known Answer Test + 자동화 테스트**를 완성한다.

---

# 0. 이번 교안의 완료 상태

Day 7이 끝났을 때 다음 명령 하나가 동작해야 한다.

```bash
./scripts/run_test.sh
```

예상 출력:

```text
[PASS] NIST XTS-AES-128 known-answer test
[PASS] 512-byte encrypt/decrypt round trip
[PASS] LBA/tweak change produces different ciphertext
[PASS] 4096-byte encrypt/decrypt round trip
[PASS] ALL TESTS
```

폴더 구조:

```text
aes_xts_golden/
├── build/
├── input/
├── output/
├── scripts/
│   ├── lba_to_tweak.py
│   └── run_test.sh
├── src/
│   └── xts_golden.c
├── vectors/
└── notes/
```

---

# Day 1 — AES의 입력/출력 구조를 정확히 잡기

## 오늘 목표

다음 질문에 답할 수 있어야 한다.

```text
AES block size는?
AES-128의 128은 무엇인가?
AES-128은 몇 round인가?
Plaintext / Ciphertext / Key의 관계는?
```

그리고 공식 FIPS AES vector 하나를 OpenSSL로 직접 검증한다.

---

## 1-1. 읽을 문서

### NIST FIPS 197

공식 PDF:

https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf

### 읽을 위치

#### 필수 1

PDF 뷰어 **9/46**, 본문 **p.1**

```text
Section 1 — Introduction
```

여기서 확인:

```text
AES-128
AES-192
AES-256

Block Size = 모두 128 bit
숫자 suffix = Key Size
```

#### 필수 2

PDF 뷰어 **10/46**, 본문 **p.2**

```text
Section 2.1 — Terms and Acronyms
```

특히:

```text
Block
Block cipher
Byte
Key
Key schedule
State
```

#### 필수 3

PDF 뷰어 **19/46**, 본문 **p.11**

```text
Section 5 — Algorithm Specifications
Table 3 — Key-Block-Round Combinations
```

표에서 직접 확인:

```text
AES-128 → Key 128, Block 128, Round 10
AES-192 → Key 192, Block 128, Round 12
AES-256 → Key 256, Block 128, Round 14
```

---

## 1-2. 환경 설치

Ubuntu / WSL2 기준:

```bash
sudo apt update
sudo apt install -y build-essential libssl-dev openssl python3 curl unzip pkg-config
```

확인:

```bash
gcc --version
openssl version
pkg-config --modversion openssl
python3 --version
```

AES-XTS 알고리즘이 OpenSSL 라이브러리에 보이는지 확인:

```bash
openssl list -cipher-algorithms | grep -i xts
```

정상 예:

```text
AES-128-XTS
AES-256-XTS
```

주의:

```text
openssl list -cipher-algorithms
```

에 XTS가 있다고 해서 반드시:

```bash
openssl enc -aes-128-xts
```

를 Golden Model에 사용한다는 뜻은 아니다.

이번 프로젝트에서는 XTS를 **EVP C API**로 사용한다.

---

## 1-3. 프로젝트 폴더 생성

```bash
mkdir -p ~/aes_xts_golden/{src,build,input,output,scripts,vectors,notes}
cd ~/aes_xts_golden
```

확인:

```bash
find . -maxdepth 2 -type d | sort
```

---

## 1-4. AES-128 단일 block 공식 vector 확인

FIPS 197에서 널리 사용하는 AES-128 example:

```text
Key:
000102030405060708090a0b0c0d0e0f

Plaintext:
00112233445566778899aabbccddeeff

Expected Ciphertext:
69c4e0d86a7b0430d8cdb78070b4c55a
```

입력 파일 생성:

```bash
python3 - <<'PY'
from pathlib import Path

Path("input/aes_plain.bin").write_bytes(
    bytes.fromhex("00112233445566778899aabbccddeeff")
)
PY
```

AES-128-ECB 한 block 암호화:

```bash
openssl enc \
  -aes-128-ecb \
  -e \
  -K 000102030405060708090a0b0c0d0e0f \
  -nopad \
  -in input/aes_plain.bin \
  -out output/aes_cipher.bin
```

결과 표시:

```bash
python3 - <<'PY'
from pathlib import Path
print(Path("output/aes_cipher.bin").read_bytes().hex())
PY
```

정상:

```text
69c4e0d86a7b0430d8cdb78070b4c55a
```

---

## 1-5. 오늘 직접 작성할 노트

`notes/day1_aes.md`:

```markdown
# Day 1 AES

- AES는 symmetric block cipher
- AES block size = 128 bit = 16 byte
- AES-128의 128 = key length
- AES-128 round = 10
- AES-192 round = 12
- AES-256 round = 14

512 B sector 안의 AES block 수:
512 / 16 = 32

4096 B 안의 AES block 수:
4096 / 16 = 256
```

---

## Day 1 통과조건

다음 네 개를 만족하면 종료한다.

```text
[ ] AES block = 16 B라고 설명 가능
[ ] AES-128의 128 = key size라고 설명 가능
[ ] FIPS AES-128 vector 결과가 MATCH
[ ] 512 B = 32 AES blocks라고 계산 가능
```

---

# Day 2 — XTS, Data Unit, Tweak, Key1/Key2

## 오늘 목표

AES와 XTS를 구분한다.

```text
AES
= 16-byte block cipher

XTS
= storage data unit을 AES로 안전하게 처리하기 위한 mode
```

---

## 2-1. 읽을 문서

### NIST SP 800-38E

PDF:

https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38e.pdf

#### 필수

PDF 뷰어 **9/12**, 본문 **p.1**

```text
Section 3 — Introduction
```

여기서 확인:

- XTS-AES는 storage device를 위한 mode
- fixed-length data unit 개념
- data in transit 용도로 설계된 것이 아님
- XTS = XEX + Ciphertext Stealing
- confidentiality 제공
- authentication 미제공

#### 필수

PDF 뷰어 **10/12**, 본문 **p.2**

```text
Section 4 — Conformance
```

여기서 확인:

```text
XTS instance =
1. Secret Key
2. Fixed Data Unit Length
3. Encrypt / Decrypt procedure
```

또한 complete block만 지원하면 CTS를 생략할 수 있다는 부분을 읽는다.

---

## 2-2. XTSVS 읽기

공식 PDF:

https://csrc.nist.gov/csrc/media/projects/cryptographic-algorithm-validation-program/documents/aes/xtsvs.pdf

### 읽을 위치

PDF 뷰어 **4/10**, 문서 p.2:

```text
Section 2 — Scope
```

확인:

```text
XTS-AES-128
→ Total XTS Key = 256 bit

XTS-AES-256
→ Total XTS Key = 512 bit
```

PDF 뷰어 **7/10**, 문서 p.5:

```text
Section 6.1
Section 6.2
```

확인:

```text
Key
Data Unit Length
Tweak 128-bit hex
또는 DataUnitSeqNumber
PT
CT
```

---

## 2-3. 반드시 그려볼 그림

종이에 직접 그린다.

### 512 B Data Unit

```text
LBA 100
┌────────────────────────────────────────────┐
│                 512 bytes                  │
└────────────────────────────────────────────┘

512 / 16 = 32 AES blocks
```

내부:

```text
┌─────┬─────┬─────┬─────────┬──────┐
│ B0  │ B1  │ B2  │   ...   │ B31  │
└─────┴─────┴─────┴─────────┴──────┘
 16B   16B   16B             16B
```

---

## 2-4. Key 구조 정리

```text
XTS-AES-128

Total Key = 32 byte

┌────────────────┬────────────────┐
│ Key1 = 16 byte │ Key2 = 16 byte │
└────────────────┴────────────────┘
```

개념적으로:

```text
Key1 → data AES
Key2 → initial tweak AES
```

---

## 2-5. Tweak를 위치정보로 이해

개념:

```text
LBA
 ↓
128-bit tweak input
 ↓
AES(Key2)
 ↓
block별 tweak 생성
 ↓
AES-XTS
```

오늘은 GF(2^128) 세부 구현까지 들어가지 않는다.

---

## 2-6. Little-endian 확인용 Python

터미널:

```bash
python3 - <<'PY'
for lba in [0, 1, 2, 0x123456789a]:
    tweak = lba.to_bytes(16, "little")
    print(hex(lba), "->", tweak.hex())
PY
```

예상:

```text
0x0 -> 00000000000000000000000000000000
0x1 -> 01000000000000000000000000000000
0x2 -> 02000000000000000000000000000000
0x123456789a -> 9a785634120000000000000000000000
```

이 byte ordering은 나중에 FPGA RTL에서도 그대로 맞춰야 한다.

---

## Day 2 통과조건

```text
[ ] AES Block과 XTS Data Unit 차이를 설명 가능
[ ] XTS-AES-128 total key = 32 B 설명 가능
[ ] Key1/Key2 역할 설명 가능
[ ] 같은 PT/Key + 다른 tweak면 CT가 달라져야 함을 설명 가능
[ ] LBA 1 → 01 00 ... 형식의 little-endian tweak를 이해
```

---

# Day 3 — OpenSSL EVP로 AES-XTS Golden Model 작성

## 오늘 목표

직접:

```text
xts_golden.c
```

를 작성하고 NIST Known Answer Test 1개를 통과한다.

---

## 3-1. 읽을 OpenSSL 문서

### AES/XTS 설명

https://docs.openssl.org/3.5/man3/EVP_aes_128_gcm/

페이지에서 검색:

```text
EVP_aes_128_xts
```

확인:

- XTS-AES는 storage용
- XTS-AES-128은 256-bit 전체 key 필요
- XTS-AES-256은 512-bit 전체 key 필요
- OpenSSL XTS는 streaming 미지원

### EVP Cipher API

https://docs.openssl.org/3.5/man3/EVP_EncryptInit/

페이지에서 다음 함수 검색:

```text
EVP_CIPHER_fetch
EVP_CIPHER_CTX_new
EVP_CipherInit_ex2
EVP_CipherUpdate
EVP_CipherFinal_ex
EVP_CIPHER_CTX_free
EVP_CIPHER_free
```

---

## 3-2. 작성할 코드 — `src/xts_golden.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

static int hex_to_bytes(const char *hex, unsigned char *out, size_t out_len)
{
    if (strlen(hex) != out_len * 2) return 0;

    for (size_t i = 0; i < out_len; ++i) {
        unsigned int v;
        if (sscanf(hex + (i * 2), "%2x", &v) != 1) return 0;
        out[i] = (unsigned char)v;
    }
    return 1;
}

static unsigned char *read_file(const char *path, size_t *len)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) return NULL;

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return NULL;
    }

    long size = ftell(fp);
    if (size < 0) {
        fclose(fp);
        return NULL;
    }

    rewind(fp);

    unsigned char *buf = malloc((size_t)size);
    if (!buf) {
        fclose(fp);
        return NULL;
    }

    if (fread(buf, 1, (size_t)size, fp) != (size_t)size) {
        free(buf);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    *len = (size_t)size;
    return buf;
}

static int write_file(const char *path,
                      const unsigned char *buf,
                      size_t len)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) return 0;

    int ok = fwrite(buf, 1, len, fp) == len;
    fclose(fp);
    return ok;
}

static int xts_crypt(int encrypt,
                     const unsigned char key[32],
                     const unsigned char tweak[16],
                     const unsigned char *in,
                     size_t in_len,
                     unsigned char *out,
                     size_t *out_len)
{
    int ok = 0;
    int len1 = 0;
    int len2 = 0;

    EVP_CIPHER *cipher =
        EVP_CIPHER_fetch(NULL, "AES-128-XTS", NULL);

    EVP_CIPHER_CTX *ctx =
        EVP_CIPHER_CTX_new();

    if (!cipher || !ctx)
        goto done;

    if (!EVP_CipherInit_ex2(
            ctx,
            cipher,
            key,
            tweak,
            encrypt,
            NULL))
        goto done;

    /*
     * XTS에서는 하나의 Data Unit을
     * 한 번의 Update 호출로 처리한다.
     */
    if (!EVP_CipherUpdate(
            ctx,
            out,
            &len1,
            in,
            (int)in_len))
        goto done;

    if (!EVP_CipherFinal_ex(
            ctx,
            out + len1,
            &len2))
        goto done;

    *out_len = (size_t)(len1 + len2);
    ok = 1;

done:
    EVP_CIPHER_CTX_free(ctx);
    EVP_CIPHER_free(cipher);
    return ok;
}

int main(int argc, char **argv)
{
    if (argc != 6) {
        fprintf(stderr,
                "usage: %s <enc|dec> "
                "<64-hex-char-key> "
                "<32-hex-char-tweak> "
                "<input> <output>\n",
                argv[0]);
        return 1;
    }

    int encrypt;

    if (strcmp(argv[1], "enc") == 0)
        encrypt = 1;
    else if (strcmp(argv[1], "dec") == 0)
        encrypt = 0;
    else {
        fprintf(stderr,
                "first argument must be enc or dec\n");
        return 1;
    }

    unsigned char key[32];
    unsigned char tweak[16];

    if (!hex_to_bytes(
            argv[2],
            key,
            sizeof(key)) ||
        !hex_to_bytes(
            argv[3],
            tweak,
            sizeof(tweak))) {
        fprintf(stderr,
                "invalid key or tweak hex string\n");
        return 1;
    }

    size_t in_len = 0;

    unsigned char *in =
        read_file(argv[4], &in_len);

    if (!in || in_len < 16) {
        fprintf(stderr,
                "input file read failed "
                "or data unit is smaller than 16 bytes\n");
        free(in);
        return 1;
    }

    unsigned char *out =
        malloc(in_len + 16);

    if (!out) {
        free(in);
        return 1;
    }

    size_t out_len = 0;

    if (!xts_crypt(
            encrypt,
            key,
            tweak,
            in,
            in_len,
            out,
            &out_len)) {
        fprintf(stderr,
                "AES-128-XTS operation failed\n");
        free(out);
        free(in);
        return 1;
    }

    if (!write_file(
            argv[5],
            out,
            out_len)) {
        fprintf(stderr,
                "output file write failed\n");
        free(out);
        free(in);
        return 1;
    }

    free(out);
    free(in);
    return 0;
}
```

---

## 3-3. 컴파일

```bash
gcc \
  -Wall \
  -Wextra \
  -O2 \
  src/xts_golden.c \
  -o build/xts_golden \
  -lcrypto
```

실행 확인:

```bash
./build/xts_golden
```

정상이라면 usage가 출력된다.

---

## 3-4. NIST XTS-AES-128 Known Answer Test

테스트용 값:

```text
Key =
a1b90cba3f06ac353b2c343876081762
090923026e91771815f29dab01932f2f

Tweak =
4faef7117cda59c66e4b92013e768ad5

Plaintext =
ebabce95b14d3c8d6fb350390790311c

Expected Ciphertext =
778ae8b43cb98d5a825081d5be471c63
```

입력/기대값 생성:

```bash
python3 - <<'PY'
from pathlib import Path

Path("vectors/nist_pt.bin").write_bytes(
    bytes.fromhex(
        "ebabce95b14d3c8d6fb350390790311c"
    )
)

Path("vectors/nist_expected_ct.bin").write_bytes(
    bytes.fromhex(
        "778ae8b43cb98d5a825081d5be471c63"
    )
)
PY
```

실행:

```bash
./build/xts_golden enc \
  a1b90cba3f06ac353b2c343876081762090923026e91771815f29dab01932f2f \
  4faef7117cda59c66e4b92013e768ad5 \
  vectors/nist_pt.bin \
  output/nist_ct.bin
```

비교:

```bash
cmp \
  output/nist_ct.bin \
  vectors/nist_expected_ct.bin
```

`cmp`가 아무것도 출력하지 않고 exit code 0이면 성공.

확인:

```bash
echo $?
```

```text
0
```

출력 hex 확인:

```bash
python3 - <<'PY'
from pathlib import Path
print(Path("output/nist_ct.bin").read_bytes().hex())
PY
```

정상:

```text
778ae8b43cb98d5a825081d5be471c63
```

---

## Day 3 통과조건

```text
[ ] xts_golden.c 직접 작성
[ ] gcc 컴파일 성공
[ ] AES-128-XTS EVP 사용
[ ] NIST Known Answer Test MATCH
```

여기까지 통과하면 Golden Model의 핵심이 만들어진 것이다.

---

# Day 4 — 512 B Sector Encrypt → Decrypt

## 오늘 목표

실제 저장장치 sector 크기와 비슷한 512 B Data Unit을 처리한다.

---

## 4-1. 512 B 입력 생성

```bash
python3 - <<'PY'
from pathlib import Path

data = bytes(i & 0xff for i in range(512))

Path("input/plaintext_512.bin").write_bytes(data)
PY
```

파일 크기 확인:

```bash
stat -c '%n %s bytes' input/plaintext_512.bin
```

정상:

```text
input/plaintext_512.bin 512 bytes
```

---

## 4-2. Key 결정

이번 주 고정 key:

```bash
KEY=000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f
```

구조:

```text
Key1:
000102030405060708090a0b0c0d0e0f

Key2:
101112131415161718191a1b1c1d1e1f
```

---

## 4-3. LBA 0용 tweak

```bash
TWEAK0=00000000000000000000000000000000
```

---

## 4-4. Encrypt

```bash
./build/xts_golden enc \
  "$KEY" \
  "$TWEAK0" \
  input/plaintext_512.bin \
  output/cipher_512_lba0.bin
```

크기 확인:

```bash
stat -c '%n %s bytes' output/cipher_512_lba0.bin
```

XTS는 data expansion이 없으므로 512 B여야 한다.

---

## 4-5. Decrypt

```bash
./build/xts_golden dec \
  "$KEY" \
  "$TWEAK0" \
  output/cipher_512_lba0.bin \
  output/decrypted_512.bin
```

---

## 4-6. 비교

```bash
cmp \
  input/plaintext_512.bin \
  output/decrypted_512.bin
```

정상:

```text
아무 출력 없음
```

확실히 표시:

```bash
if cmp -s \
  input/plaintext_512.bin \
  output/decrypted_512.bin
then
    echo PASS
else
    echo FAIL
fi
```

---

## 4-7. Hash 비교

```bash
sha256sum \
  input/plaintext_512.bin \
  output/decrypted_512.bin
```

두 hash가 같아야 한다.

---

## Day 4 통과조건

```text
[ ] 512 B 입력 생성
[ ] Encrypt 성공
[ ] Ciphertext 크기 512 B
[ ] Decrypt 성공
[ ] cmp PASS
[ ] sha256 동일
```

---

# Day 5 — NIST XTSVS와 공식 Vector 파일 구조 이해

## 오늘 목표

단순히 “복호화가 되네”에서 끝내지 않고:

```text
Known Answer Test
```

의 의미를 정확히 이해한다.

---

## 5-1. XTSVS 재독

PDF:

https://csrc.nist.gov/csrc/media/projects/cryptographic-algorithm-validation-program/documents/aes/xtsvs.pdf

읽기:

```text
Section 5
Section 6
Section 6.1
Section 6.2
```

특히 확인:

```text
REQUEST
RESPONSE
Expected result
PASS / FAIL
```

핵심 사고:

```text
내 Encrypt
→ 내 Decrypt
→ 원본 복구
```

만으로는 부족하다.

반드시:

```text
표준에서 정해진 입력
↓
내 구현
↓
표준 기대 출력과 비교
```

가 필요하다.

---

## 5-2. NIST 공식 XTS Vector 다운로드

CAVP 페이지:

https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/CAVP-TESTING-BLOCK-CIPHER-MODES

해당 페이지에서:

```text
XTS-AES Test Vectors
```

를 선택한다.

직접 다운로드 명령을 사용할 경우:

```bash
curl -L \
  'https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Algorithm-Validation-Program/documents/aes/XTSTestVectors.zip' \
  -o vectors/XTSTestVectors.zip
```

압축 해제:

```bash
mkdir -p vectors/nist_xts

unzip \
  vectors/XTSTestVectors.zip \
  -d vectors/nist_xts
```

파일 확인:

```bash
find vectors/nist_xts \
  -type f \
  -name 'XTSGenAES*.rsp' \
  -print
```

대략 다음 종류가 있다.

```text
XTSGenAES128.rsp
XTSGenAES256.rsp

tweak input:
- 128-bit hex string
- data unit sequence number
```

---

## 5-3. Vector 파일 열어보기

```bash
less \
"vectors/nist_xts/format tweak value input - 128 hex str/XTSGenAES128.rsp"
```

파일 안에서 다음 항목을 찾는다.

```text
[ENCRYPT]

COUNT =
DataUnitLen =
Key =
i =
PT =
CT =
```

검색:

```text
/ENCRYPT
```

또는:

```bash
grep -n \
  '^\[ENCRYPT\]\|^COUNT\|^DataUnitLen\|^Key\|^i\|^PT\|^CT' \
  "vectors/nist_xts/format tweak value input - 128 hex str/XTSGenAES128.rsp" \
  | head -40
```

---

## 5-4. NIST Vector 하나를 직접 내 프로그램에 넣기

Vector 하나를 선택하여:

```text
Key
i
PT
CT
```

를 복사한다.

PT 파일:

```bash
python3 - <<'PY'
from pathlib import Path

pt_hex = "여기에_NIST_PT"

Path("vectors/my_nist_pt.bin").write_bytes(
    bytes.fromhex(pt_hex)
)
PY
```

실행:

```bash
./build/xts_golden enc \
  NIST_KEY \
  NIST_I \
  vectors/my_nist_pt.bin \
  output/my_nist_ct.bin
```

기대 CT 파일을 만든 후 `cmp`한다.

---

## Day 5 통과조건

```text
[ ] NIST XTS Test Vector ZIP 위치를 안다
[ ] XTSGenAES128.rsp 파일을 열어봤다
[ ] Key / i / PT / CT의 의미를 안다
[ ] 최소 NIST vector 1개를 내 Golden Model에서 MATCH
[ ] Round-trip과 Known Answer Test 차이를 설명 가능
```

---

# Day 6 — LBA → Tweak + LBA 0/1 + 4096 B

## 오늘 목표

저장장치 프로젝트와 직접 연결한다.

```text
LBA
 ↓
Tweak
 ↓
AES-XTS
```

를 코드로 만든다.

---

## 6-1. `scripts/lba_to_tweak.py`

```python
#!/usr/bin/env python3
import sys

if len(sys.argv) != 2:
    raise SystemExit(
        f"usage: {sys.argv[0]} <LBA>"
    )

lba = int(sys.argv[1], 0)

if not 0 <= lba < (1 << 128):
    raise SystemExit(
        "LBA/tweak value must fit in 128 bits"
    )

print(
    lba.to_bytes(
        16,
        byteorder="little"
    ).hex()
)
```

실행권한:

```bash
chmod +x scripts/lba_to_tweak.py
```

---

## 6-2. 확인

```bash
./scripts/lba_to_tweak.py 0
./scripts/lba_to_tweak.py 1
./scripts/lba_to_tweak.py 2
./scripts/lba_to_tweak.py 0x123456789a
```

정상 예:

```text
00000000000000000000000000000000
01000000000000000000000000000000
02000000000000000000000000000000
9a785634120000000000000000000000
```

---

## 6-3. LBA 0 / LBA 1 비교

```bash
KEY=000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f

TWEAK0=$(./scripts/lba_to_tweak.py 0)
TWEAK1=$(./scripts/lba_to_tweak.py 1)
```

LBA 0:

```bash
./build/xts_golden enc \
  "$KEY" \
  "$TWEAK0" \
  input/plaintext_512.bin \
  output/cipher_512_lba0.bin
```

LBA 1:

```bash
./build/xts_golden enc \
  "$KEY" \
  "$TWEAK1" \
  input/plaintext_512.bin \
  output/cipher_512_lba1.bin
```

비교:

```bash
if cmp -s \
  output/cipher_512_lba0.bin \
  output/cipher_512_lba1.bin
then
    echo FAIL
else
    echo PASS
fi
```

정상:

```text
PASS
```

---

## 6-4. 4096 B 생성

```bash
python3 - <<'PY'
from pathlib import Path

Path("input/plaintext_4096.bin").write_bytes(
    bytes(i & 0xff for i in range(4096))
)
PY
```

---

## 6-5. 4096 B를 하나의 Data Unit으로 테스트

```bash
./build/xts_golden enc \
  "$KEY" \
  "$TWEAK0" \
  input/plaintext_4096.bin \
  output/cipher_4096.bin
```

복호화:

```bash
./build/xts_golden dec \
  "$KEY" \
  "$TWEAK0" \
  output/cipher_4096.bin \
  output/decrypted_4096.bin
```

비교:

```bash
cmp \
  input/plaintext_4096.bin \
  output/decrypted_4096.bin
```

---

## 6-6. 매우 중요한 개념

다음 두 경우는 다르다.

### 경우 A

```text
4096 B 전체
=
하나의 XTS Data Unit
```

→ tweak 하나로 4096 B 처리.

### 경우 B

```text
512 B sector × 8
=
8개의 XTS Data Unit
```

→ LBA 0~7처럼 각각 다른 tweak를 사용.

```text
Sector 0 → Tweak 0
Sector 1 → Tweak 1
Sector 2 → Tweak 2
...
Sector 7 → Tweak 7
```

FPGA/SSD 프로젝트 전에 어느 방식을 채택할지 인터페이스 사양에 명시해야 한다.

초기 프로젝트에서는 보통:

```text
1 logical sector = 1 XTS Data Unit
```

처럼 단순하게 잡는 것이 이해하기 쉽다.

---

## Day 6 통과조건

```text
[ ] LBA → little-endian 16 B tweak 변환 가능
[ ] LBA 0 / LBA 1 cipher가 다름
[ ] 4096 B round-trip PASS
[ ] 4 KiB 1 Data Unit과 512 B × 8 차이 설명 가능
```

---

# Day 7 — 모든 테스트 자동화

## 오늘 목표

수작업 명령을 그만 치고:

```bash
./scripts/run_test.sh
```

하나로 검증한다.

---

## 7-1. `scripts/run_test.sh`

```bash
#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

mkdir -p \
  build \
  input \
  output \
  vectors

gcc \
  -Wall \
  -Wextra \
  -O2 \
  src/xts_golden.c \
  -o build/xts_golden \
  -lcrypto

KEY_FIXED="000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"

TWEAK0="$(
    python3 scripts/lba_to_tweak.py 0
)"

TWEAK1="$(
    python3 scripts/lba_to_tweak.py 1
)"

# --------------------------------------------------
# 1. NIST XTS-AES-128 Known Answer Test
# --------------------------------------------------

NIST_KEY="a1b90cba3f06ac353b2c343876081762090923026e91771815f29dab01932f2f"

NIST_TWEAK="4faef7117cda59c66e4b92013e768ad5"

python3 - <<'PY'
from pathlib import Path

Path("vectors/nist_pt.bin").write_bytes(
    bytes.fromhex(
        "ebabce95b14d3c8d6fb350390790311c"
    )
)

Path("vectors/nist_expected_ct.bin").write_bytes(
    bytes.fromhex(
        "778ae8b43cb98d5a825081d5be471c63"
    )
)
PY

./build/xts_golden enc \
  "$NIST_KEY" \
  "$NIST_TWEAK" \
  vectors/nist_pt.bin \
  output/nist_ct.bin

cmp -s \
  output/nist_ct.bin \
  vectors/nist_expected_ct.bin

echo \
"[PASS] NIST XTS-AES-128 known-answer test"

# --------------------------------------------------
# 2. Test Input 생성
# --------------------------------------------------

python3 - <<'PY'
from pathlib import Path

Path(
    "input/plaintext_512.bin"
).write_bytes(
    bytes(i & 0xff for i in range(512))
)

Path(
    "input/plaintext_4096.bin"
).write_bytes(
    bytes(i & 0xff for i in range(4096))
)
PY

# --------------------------------------------------
# 3. 512 B Round Trip
# --------------------------------------------------

./build/xts_golden enc \
  "$KEY_FIXED" \
  "$TWEAK0" \
  input/plaintext_512.bin \
  output/cipher_512_lba0.bin

./build/xts_golden dec \
  "$KEY_FIXED" \
  "$TWEAK0" \
  output/cipher_512_lba0.bin \
  output/decrypted_512.bin

cmp -s \
  input/plaintext_512.bin \
  output/decrypted_512.bin

echo \
"[PASS] 512-byte encrypt/decrypt round trip"

# --------------------------------------------------
# 4. LBA/Tweak Difference Test
# --------------------------------------------------

./build/xts_golden enc \
  "$KEY_FIXED" \
  "$TWEAK1" \
  input/plaintext_512.bin \
  output/cipher_512_lba1.bin

if cmp -s \
  output/cipher_512_lba0.bin \
  output/cipher_512_lba1.bin
then
    echo \
    "[FAIL] LBA 0 and LBA 1 ciphertext are identical"

    exit 1
fi

echo \
"[PASS] LBA/tweak change produces different ciphertext"

# --------------------------------------------------
# 5. 4096 B Round Trip
# --------------------------------------------------

./build/xts_golden enc \
  "$KEY_FIXED" \
  "$TWEAK0" \
  input/plaintext_4096.bin \
  output/cipher_4096.bin

./build/xts_golden dec \
  "$KEY_FIXED" \
  "$TWEAK0" \
  output/cipher_4096.bin \
  output/decrypted_4096.bin

cmp -s \
  input/plaintext_4096.bin \
  output/decrypted_4096.bin

echo \
"[PASS] 4096-byte encrypt/decrypt round trip"

echo \
"[PASS] ALL TESTS"
```

실행권한:

```bash
chmod +x scripts/run_test.sh
```

실행:

```bash
./scripts/run_test.sh
```

---

# 7-2. 정상 결과

```text
[PASS] NIST XTS-AES-128 known-answer test
[PASS] 512-byte encrypt/decrypt round trip
[PASS] LBA/tweak change produces different ciphertext
[PASS] 4096-byte encrypt/decrypt round trip
[PASS] ALL TESTS
```

---

# Day 7 최종 통과조건

```text
[ ] clean build 가능
[ ] NIST KAT PASS
[ ] 512 B round-trip PASS
[ ] LBA 0/1 difference PASS
[ ] 4096 B round-trip PASS
[ ] 한 명령으로 전체 테스트 실행
```

---

# 8. 7일 종료 후 만들어진 Golden Model의 역할

다음 FPGA 단계에서:

```text
Test Vector
    │
    ├──── OpenSSL C Golden Model
    │          ↓
    │       CT_SW
    │
    └──── FPGA AES-XTS RTL
               ↓
            CT_FPGA
```

비교:

```text
CT_SW == CT_FPGA
```

를 수행한다.

이때 문제가 발생하면 다음 순서로 범위를 줄인다.

```text
NIST KAT
  ↓
AES Core
  ↓
XTS Tweak
  ↓
Endian
  ↓
LBA
  ↓
AXI Stream
  ↓
DMA
```

---

# 9. 이번 주에 절대 확장하지 않을 것

아래 항목은 지금 하지 않는다.

```text
AES-256-XTS 추가
CTS 추가
GUI 추가
Linux Driver 연결
AXI DMA 연결
FPGA RTL 구현
성능 최적화
```

Day 7의 `ALL PASS`가 먼저다.

---

# 10. 하루 4시간 기준 권장 시간배분

## Day 1~2

```text
공식 문서       1시간 30분
직접 정리          30분
실습             1시간 30분
복습               30분
```

## Day 3~7

```text
문서 확인           30분
코드 작성         2시간
테스트/디버깅     1시간
정리               30분
```

---

# 11. 참고 문서

## NIST

FIPS 197:

https://csrc.nist.gov/pubs/fips/197/final

FIPS 197 PDF:

https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf

SP 800-38E:

https://csrc.nist.gov/pubs/sp/800/38/e/final

SP 800-38E PDF:

https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38e.pdf

XTSVS:

https://csrc.nist.gov/csrc/media/projects/cryptographic-algorithm-validation-program/documents/aes/xtsvs.pdf

CAVP XTS Test Vectors:

https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/CAVP-TESTING-BLOCK-CIPHER-MODES

## OpenSSL

AES / XTS EVP:

https://docs.openssl.org/3.5/man3/EVP_aes_128_gcm/

EVP Cipher routines:

https://docs.openssl.org/3.5/man3/EVP_EncryptInit/

`openssl enc`:

https://docs.openssl.org/3.5/man1/openssl-enc/

## XTS tweak byte ordering 참고

Intel Cryptography Primitives:

https://www.intel.com/content/www/us/en/docs/crypto-primitives-library/developer-guide-reference/2025-2/aesencryptxts-direct-aesdecryptxts-direct.html

---

# 12. 중요 메모

이 교안의 C 코드와 `run_test.sh`는 OpenSSL 3.x 계열을 기준으로 작성했다.

핵심 사용 API:

```text
EVP_CIPHER_fetch()
EVP_CipherInit_ex2()
```

OpenSSL 1.1.1 환경이라면 API가 조금 달라진다.

그 경우에는 이 코드를 억지로 수정하지 말고 OpenSSL 버전을 먼저 확인한다.

```bash
openssl version
```

그 후 1.1.1용 버전으로 별도 변경하는 것이 좋다.

---

작성 기준일: 2026-08-24
