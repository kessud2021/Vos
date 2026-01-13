/**
 * VSS-CO OS Kernel Test Harness
 * 
 * Runs kernel code on Windows without virtualization
 * Simulates hardware and tests kernel functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

/* Forward declarations */
extern void kernel_main(void);

/* Kernel stubs for testing */
void pr_info(const char *fmt, ...) {
    printf("[INFO] ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void pr_err(const char *fmt, ...) {
    printf("[ERROR] ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void pr_debug(const char *fmt, ...) {
    #ifdef DEBUG
    printf("[DEBUG] ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    #endif
}

void pr_panic(const char *fmt, ...) {
    printf("[PANIC] ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\nKernel halted.\n");
    exit(1);
}

void halt(void) {
    printf("[HALT] System halted.\n");
    exit(0);
}

/* Test main */
int main(void) {
    printf("========================================\n");
    printf("VSS-CO OS Kernel Test Harness\n");
    printf("========================================\n\n");
    
    printf("System Info:\n");
    printf("  Arch: x86-64\n");
    printf("  Mode: Test/Simulation\n");
    printf("  Date: %s\n\n", __DATE__);
    
    printf("Running kernel_main()...\n\n");
    fflush(stdout);
    
    /* Run kernel */
    kernel_main();
    
    printf("\n========================================\n");
    printf("Test completed.\n");
    printf("========================================\n");
    
    return 0;
}
