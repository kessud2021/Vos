/**
 * Process management stubs
 */

#include <kernel.h>

pid_t do_fork(void) {
    /* TODO: Implement fork */
    return 1;
}

int do_exec(const char *filename, char *const argv[]) {
    /* TODO: Implement exec */
    return 0;
}

void do_exit(int code) {
    /* TODO: Implement exit */
}
