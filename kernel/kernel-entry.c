/**
 * Kernel entry point wrapper
 */

extern void kernel_main(void);
extern void _start(void);

/* Minimal entry stub */
void _entry(void) {
    kernel_main();
}
