# BBB custom stage-1 bootloader

This project creates a custom AM335x `MLO` that is loaded directly by the
AM335x internal Boot ROM from a FAT-formatted microSD card.

## Scope

Implemented:

- GP image header generation
- execution from AM335x internal SRAM at `0x402F0400`
- ARM exception vector base installation
- SVC stack and `.bss` initialization
- watchdog disable
- UART0 pinmux, clock and 115200-8-N-1 initialization
- `BBB stage1 alive` output

Not implemented yet:

- DDR3 initialization
- MMC0/SD block driver
- FAT parser
- second-stage loading
- RTOS scheduler

## Build

Required toolchain:

```bash
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi make python3 dosfstools
make
make inspect
```

Expected outputs:

```text
stage1.elf
stage1.bin
MLO
stage1.map
```

`MLO` layout:

```text
offset 0x00: uint32 little-endian payload size
offset 0x04: uint32 little-endian destination/entry = 0x402F0400
offset 0x08: stage1.bin
```

## Prepare microSD on Linux/WSL

**Replace `/dev/sdX` with the SD card device. A wrong device destroys data.**
Check it with `lsblk -p` before writing.

```bash
sudo umount /dev/sdX?* 2>/dev/null || true
sudo fdisk /dev/sdX
```

Inside `fdisk`, create an MBR/DOS partition table:

```text
o       # new DOS/MBR table
n       # new partition
p       # primary
1       # partition 1
<Enter> # default first sector
+64M    # enough for stage 1 development
a       # toggle bootable flag
1       # partition 1
t       # change type
c       # W95 FAT32 (LBA)
w       # write
```

Then format and copy:

```bash
sudo mkfs.vfat -F 32 -n BOOT /dev/sdX1
sudo mkdir -p /mnt/bbbboot
sudo mount /dev/sdX1 /mnt/bbbboot
sudo cp MLO /mnt/bbbboot/MLO
sync
sudo umount /mnt/bbbboot
```

Copy `MLO` first and keep it in the root directory.

## UART wiring

Use the BBB debug serial header and a **3.3 V TTL** USB-UART adapter.
Do not connect RS-232 voltage levels.

```text
BBB J1 pin 1 GND -> adapter GND
BBB J1 pin 4 RX  -> adapter TX
BBB J1 pin 5 TX  -> adapter RX
```

Terminal:

```text
115200 baud, 8 data bits, no parity, 1 stop bit, no flow control
```

Linux example:

```bash
picocom -b 115200 /dev/ttyUSB0
```

## Boot test

1. Remove BBB power.
2. Insert microSD.
3. Hold the BBB S2/BOOT button.
4. Apply power while holding S2.
5. Release S2 after power-up begins.
6. Check the UART terminal.

Expected message:

```text
BBB stage1 alive
```

## Correct next milestone

After this message is reliable, extend in this order:

1. exception handlers that print the fault type
2. GPIO user-LED driver
3. PMIC/I2C0 and PLL setup
4. BBB DDR3 initialization
5. MMC0 PIO block read
6. load a raw second-stage image from a fixed LBA into DDR
7. jump to the second-stage entry point
8. implement timer IRQ, context switch and RTOS tasks in stage 2

Do not begin with a FAT parser. A fixed-LBA raw second stage is much easier to
debug and keeps the first-stage bootloader small.
