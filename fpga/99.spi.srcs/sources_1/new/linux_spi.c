#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

int main(void)
{
    const char *device = "/dev/spidev0.0";

    int fd;
    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = 1000000;   // 1 MHz

    uint8_t tx[] = {0xA5, 0x00, 0xFF, 0x3C, 0xA5};
    uint8_t rx[sizeof(tx)] = {0};

    fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    /* SPI Mode 0 */
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
        perror("SPI_IOC_WR_MODE");
        close(fd);
        return 1;
    }

    /* 8 bit */
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        perror("SPI_IOC_WR_BITS_PER_WORD");
        close(fd);
        return 1;
    }

    /* 1 MHz */
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        perror("SPI_IOC_WR_MAX_SPEED_HZ");
        close(fd);
        return 1;
    }

    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof(tr));

    tr.tx_buf = (uintptr_t)tx;
    tr.rx_buf = (uintptr_t)rx;
    tr.len = sizeof(tx);
    tr.speed_hz = speed;
    tr.bits_per_word = bits;

    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 0) {
        perror("SPI_IOC_MESSAGE");
        close(fd);
        return 1;
    }

    if (ret != (int)sizeof(tx)) {
        fprintf(stderr, "Short transfer: %d / %zu bytes\n", ret, sizeof(tx));
        close(fd);
        return 1;
    }

    int failed = 0;
    for (size_t i = 0; i < sizeof(tx); ++i) {
        printf("[%zu] TX=0x%02X RX=0x%02X %s\n", i, tx[i], rx[i],
               rx[i] == 0x55 ? "PASS" : "FAIL (expected 0x55)");
        if (rx[i] != 0x55)
            failed = 1;
    }
    close(fd);

    return failed;
}
