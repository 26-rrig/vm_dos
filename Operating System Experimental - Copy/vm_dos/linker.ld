ENTRY(_start)

SECTIONS {
    . = 0x100000;

    .multiboot ALIGN(4) : {
        KEEP(*(.multiboot))
    }

    .text ALIGN(16) : {
        *(.text*)
    }

    .rodata ALIGN(32) : {
        *(.rodata*)
    }

    .data ALIGN(32) : {
        *(.data*)
    }

    .bss ALIGN(32) : {
        *(.bss*)
        *(COMMON)
    }
}
