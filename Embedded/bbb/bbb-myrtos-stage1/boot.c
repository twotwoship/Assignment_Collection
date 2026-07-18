#include <stdint.h>

#define REG32(addr) (*(volatile uint32_t *)(uintptr_t)(addr))

#define CM_WKUP_BASE               0x44E00400u
#define CM_WKUP_UART0_CLKCTRL      (CM_WKUP_BASE + 0xB4u)

#define CONTROL_MODULE_BASE        0x44E10000u
#define CONF_UART0_RXD             (CONTROL_MODULE_BASE + 0x970u)
#define CONF_UART0_TXD             (CONTROL_MODULE_BASE + 0x974u)

#define WDT1_BASE                  0x44E35000u
#define WDT_WWPS                   (WDT1_BASE + 0x34u)
#define WDT_WSPR                   (WDT1_BASE + 0x48u)
#define W_PEND_WSPR                (1u << 4)

#define UART0_BASE                 0x44E09000u
#define UART_RHR_THR_DLL           (UART0_BASE + 0x00u)
#define UART_IER_DLH               (UART0_BASE + 0x04u)
#define UART_IIR_FCR_EFR           (UART0_BASE + 0x08u)
#define UART_LCR                   (UART0_BASE + 0x0Cu)
#define UART_LSR                   (UART0_BASE + 0x14u)
#define UART_MDR1                  (UART0_BASE + 0x20u)
#define UART_SYSC                  (UART0_BASE + 0x54u)
#define UART_SYSS                  (UART0_BASE + 0x58u)

static void watchdog_disable(void)
{
    REG32(WDT_WSPR) = 0xAAAAu;
    while ((REG32(WDT_WWPS) & W_PEND_WSPR) != 0u) { }

    REG32(WDT_WSPR) = 0x5555u;
    while ((REG32(WDT_WWPS) & W_PEND_WSPR) != 0u) { }
}

static void uart0_init(void)
{
    /* Enable UART0 module clock and wait until functional. */
    REG32(CM_WKUP_UART0_CLKCTRL) = 0x2u;
    while (((REG32(CM_WKUP_UART0_CLKCTRL) >> 16) & 0x3u) != 0u) { }

    /* Mode 0. RX: receiver enabled + pull-up. TX: output, pull disabled. */
    REG32(CONF_UART0_RXD) = 0x30u;
    REG32(CONF_UART0_TXD) = 0x08u;

    /* Soft-reset UART. */
    REG32(UART_SYSC) = 0x2u;
    while ((REG32(UART_SYSS) & 0x1u) == 0u) { }

    /* Disable while configuring. */
    REG32(UART_MDR1) = 0x7u;

    /* Configuration mode B: enable enhanced register access. */
    REG32(UART_LCR) = 0xBFu;
    REG32(UART_IIR_FCR_EFR) |= 0x10u;

    /* Divisor latch access, 48 MHz / (16 * 115200) ~= 26. */
    REG32(UART_LCR) = 0x80u;
    REG32(UART_RHR_THR_DLL) = 26u;
    REG32(UART_IER_DLH) = 0u;

    /* 8 data bits, no parity, 1 stop bit. */
    REG32(UART_LCR) = 0x03u;

    /* Enable and clear TX/RX FIFOs. */
    REG32(UART_IIR_FCR_EFR) = 0x07u;

    /* UART 16x mode. */
    REG32(UART_MDR1) = 0x0u;
}

static void uart0_putc(char c)
{
    while ((REG32(UART_LSR) & (1u << 5)) == 0u) { }
    REG32(UART_RHR_THR_DLL) = (uint32_t)(uint8_t)c;
}

static void uart0_puts(const char *s)
{
    while (*s != '\0') {
        if (*s == '\n') {
            uart0_putc('\r');
        }
        uart0_putc(*s++);
    }
}

void boot_main(void)
{
    watchdog_disable();
    uart0_init();
    uart0_puts("BBB stage1 alive\n");

    for (;;) {
        __asm__ volatile ("wfi");
    }
}
