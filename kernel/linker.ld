/* x86-64 kernel linker script */

ENTRY(_start)

SECTIONS {
    /* Kernel base address (higher half kernel: 0xffffffff80000000) */
    . = 0x100000;  /* 1MB load address */

    .text ALIGN(0x1000) : {
        *(.multiboot)
        *(.text)
        *(.text.*)
        . = ALIGN(0x1000);
    }

    .rodata ALIGN(0x1000) : {
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(0x1000);
    }

    .data ALIGN(0x1000) : {
        *(.data)
        *(.data.*)
        . = ALIGN(0x1000);
    }

    .bss ALIGN(0x1000) : {
        *(COMMON)
        *(.bss)
        *(.bss.*)
        . = ALIGN(0x1000);
    }

    /DISCARD/ : {
        *(.note.GNU-stack)
        *(.gnu_debuglink)
        *(.gnu.lto_*)
    }
}
