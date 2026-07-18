.syntax unified
.cpu cortex-a8
.arm

.section .vectors, "ax", %progbits
.balign 32
.global _start
.type _start, %function
_start:
    b reset_handler
    b default_handler
    b default_handler
    b default_handler
    b default_handler
    b default_handler
    b default_handler
    b default_handler

.section .text.reset, "ax", %progbits
reset_handler:
    cpsid if

    /* Enter SVC mode. */
    mrs r0, cpsr
    bic r0, r0, #0x1f
    orr r0, r0, #0x13
    msr cpsr_c, r0

    /* Keep the first milestone simple: MMU and caches disabled. */
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #1
    bic r0, r0, #4
    bic r0, r0, #0x1000
    mcr p15, 0, r0, c1, c0, 0
    dsb
    isb

    ldr sp, =__stack_top

    /* Install our exception vector base. */
    ldr r0, =_start
    mcr p15, 0, r0, c12, c0, 0
    dsb
    isb

    /* Clear .bss. */
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    mov r2, #0
1:
    cmp r0, r1
    strlo r2, [r0], #4
    blo 1b

    bl boot_main

2:
    wfi
    b 2b

default_handler:
    b default_handler
