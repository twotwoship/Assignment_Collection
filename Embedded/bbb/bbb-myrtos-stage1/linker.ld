ENTRY(_start)

/*
 * AM335x GP-device public SRAM area used for the downloaded image.
 * Keep 0x4030A000-0x4030AFFF for our SVC stack and stay below
 * the ROM-reserved region beginning near 0x4030B800.
 */
MEMORY
{
    SRAM (rwx) : ORIGIN = 0x402F0400, LENGTH = 0x00019C00
}

SECTIONS
{
    . = ORIGIN(SRAM);

    .vectors : ALIGN(32)
    {
        KEEP(*(.vectors))
    } > SRAM

    .text : ALIGN(4)
    {
        *(.text.reset)
        *(.text*)
        *(.rodata*)
    } > SRAM

    .data : ALIGN(4)
    {
        *(.data*)
    } > SRAM

    .bss (NOLOAD) : ALIGN(4)
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > SRAM

    . = ALIGN(8);
    __image_end__ = .;

    __stack_bottom = 0x4030A000;
    __stack_top    = 0x4030B000;

    ASSERT(__image_end__ <= __stack_bottom,
           "stage1 image overlaps its SRAM stack")

    /DISCARD/ :
    {
        *(.comment*)
        *(.note*)
        *(.eh_frame*)
        *(.ARM.exidx*)
        *(.ARM.extab*)
    }
}
