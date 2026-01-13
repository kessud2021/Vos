# x86-64 interrupt handlers

.section .text

.global interrupt_handler

interrupt_handler:
    # TODO: Save registers, call C handler
    ret
