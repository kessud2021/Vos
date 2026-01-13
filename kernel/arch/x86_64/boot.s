# x86-64 bootloader entry point  
.code64

.section .text
    .globl _start

_start:
    # Set up stack
    lea stack_top(%rip), %rsp
    
    # Just return (kernel_main called separately)
    ret

.section .data
    .align 16
    .skip 8192
stack_top:
