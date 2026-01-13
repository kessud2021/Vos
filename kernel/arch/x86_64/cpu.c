/**
 * x86-64 CPU initialization
 */

void init_cpu_x86_64(void) {
    /* TODO: Setup GDT, IDT, TSS */
}

void halt(void) {
    /* Disable interrupts and halt */
    __asm__ volatile("cli");
    while (1) {
        __asm__ volatile("hlt");
    }
}
