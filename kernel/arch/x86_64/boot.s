; x86-64 bootloader entry point
; Assumes multiboot2 boot (GRUB2)

extern kernel_main

section .text
    global _start:function

_start:
    ; EBX = multiboot info pointer
    ; Set up stack
    mov rsp, stack_top

    ; Call kernel
    call kernel_main

    ; Halt
    cli
    hlt
    jmp $

section .data
    align 16
    stack: resq 1024
    stack_top:
