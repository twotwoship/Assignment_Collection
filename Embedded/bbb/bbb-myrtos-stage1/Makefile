CROSS_COMPILE ?= arm-none-eabi-
CC      := $(CROSS_COMPILE)gcc
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
SIZE    := $(CROSS_COMPILE)size
PYTHON  ?= python3

CPUFLAGS := -mcpu=cortex-a8 -marm -mfloat-abi=soft
COMMON   := $(CPUFLAGS) -ffreestanding -fno-builtin -fno-stack-protector \
            -fno-pic -fno-unwind-tables -fno-asynchronous-unwind-tables \
            -ffunction-sections -fdata-sections -Os -Wall -Wextra
CFLAGS   := $(COMMON) -std=c11
ASFLAGS  := $(COMMON)
LDFLAGS  := $(CPUFLAGS) -nostdlib -nostartfiles -T linker.ld \
            -Wl,--gc-sections -Wl,-Map=stage1.map

.PHONY: all clean inspect
all: MLO

startup.o: startup.S
	$(CC) $(ASFLAGS) -c $< -o $@

boot.o: boot.c
	$(CC) $(CFLAGS) -c $< -o $@

stage1.elf: startup.o boot.o linker.ld
	$(CC) $(LDFLAGS) startup.o boot.o -o $@

stage1.bin: stage1.elf
	$(OBJCOPY) -O binary $< $@

MLO: stage1.bin mk_gp_header.py
	$(PYTHON) mk_gp_header.py stage1.bin MLO
	$(SIZE) stage1.elf
	@echo "MLO size: $$(stat -c%s MLO) bytes"

inspect: stage1.elf MLO
	$(OBJDUMP) -h -d stage1.elf > stage1.dump
	@od -An -tx4 -N8 MLO
	@echo "First word = payload size, second word = 0x402F0400"

clean:
	rm -f *.o *.elf *.bin MLO *.map *.dump
