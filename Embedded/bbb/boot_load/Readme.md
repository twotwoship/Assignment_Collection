# BeagleBone Black Boot Notes

These notes were written for the BeagleBone Black (BBB).

## Prerequisites and Steps Using U-Boot

1. Prepare an Ubuntu environment. The author's setup used Ubuntu 26.04 on WSL2.
2. Install a cross-compiler for the BBB.
3. Install the packages below. Other packages were installed as needed and were not fully recorded:

   ```text
   autoconf automake bison bzip2 cmake flex g++ gawk gcc gettext git gperf help2man libncurses5-dev
   libstdc++6 libtool libtool-bin make patch python3-dev rsync texinfo unzip wget xz-utils
   ```

4. Create two partitions on the SD card: a 64 MiB FAT32 first partition and a 1 GiB ext4 second partition.
5. Build U-Boot 2021.01 with the cross-compiler. Copy the generated `MLO` and `u-boot.img` files to the first SD card partition. Example configuration command:

   ```bash
   make ARCH=arm CROSS_COMPILE=arm-cortex_a8-linux-gnueabi- am335x_evm_defconfig
   ```

6. Connect a serial debug adapter to the BBB's J1 UART header. The original setup notes mention an adapter recommended by the board vendor or a locally available alternative.
7. Open a serial terminal. The author's setup used MobaXterm with 115200 baud, 8 data bits, no parity, 1 stop bit, and no flow control.
8. With the terminal open and SD card inserted, hold S2 while applying power. Press a key promptly to interrupt autoboot and reach the U-Boot prompt.
9. Once the prompt is available, continue with a bare-metal program, an RTOS, or another boot target.

## Boot Concepts

### Stage 1: ROM Code

The AM335x contains manufacturer-provided Boot ROM code. It loads the SPL (Secondary Program Loader, packaged as `MLO` in this setup) into internal SRAM, noted here as 64 KiB, then jumps to the SPL entry point.

### Stage 2: SPL

The SPL runs from SRAM and loads `u-boot.img` into DRAM. DDR must be initialized before that image can run from DRAM.

### Stage 3: U-Boot

U-Boot can load a kernel into DRAM, optionally together with an FDT and an initial RAM disk. DDR must already be initialized before DRAM is used.

The original notes refer to `u-boot.img` as “TPL”; that label is retained here only as context for the notes.

![BeagleBone Black boot screenshot](../../../img/BBB_BOOT_LOAD.png)

## Recorded Boot Sequence

1. `MLO` initializes DDR, noted here as 512 MB, and loads the accompanying `u-boot.img`.
2. `u-boot.img` runs and provides a command prompt.

## Options to Explore

- Write a custom `MLO` that initializes DDR and launches the next stage.
- Use U-Boot to launch a custom RTOS, a bare-metal program, or Linux.
