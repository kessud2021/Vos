/**
 * @file process.c
 * @brief VOS process management example
 */

#include <library/Vos.h>

int main(void) {
    vos_info("Process Management Example");
    vos_info("==========================");
    
    vos_pid_t parent_pid = vos_getpid();
    vos_info("Parent PID: %d", parent_pid);
    
    // Fork a child process
    vos_pid_t pid = vos_fork();
    
    if (pid == 0) {
        // Child process
        vos_info("[CHILD] Process ID: %d", vos_getpid());
        vos_info("[CHILD] Parent PID: %d", vos_getppid());
        vos_info("[CHILD] About to exit...");
        vos_exit(42);  /* Exit with code 42 */
    } else if (pid > 0) {
        // Parent process
        vos_info("[PARENT] Forked child with PID: %d", pid);
        vos_info("[PARENT] Waiting for child...");
        
        int status = 0;
        vos_pid_t reaped = vos_waitpid(pid, &status);
        vos_info("[PARENT] Child %d exited with status %d", reaped, status);
    } else {
        // Error
        vos_err("Fork failed!");
    }
    
    vos_info("Example complete");
    return 0;
}
