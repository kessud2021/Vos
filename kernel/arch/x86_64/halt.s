.code64
.globl halt

halt:
    cli
    hlt
    jmp halt
