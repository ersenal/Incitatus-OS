OUTPUT_FORMAT("binary")
ENTRY(main)
SECTIONS
{

    .text 0x300000: AT(0)
    {

        *(.text)
        . = ALIGN(4);
        *(.eh_frame)
        . = ALIGN(4);
        *(.data)
        . = ALIGN(4);
        *(.rodata)
        . = ALIGN(4);
        *(.bss)
        . = ALIGN(4);
        *(.COMMON)
        . = ALIGN(4);

    }

}