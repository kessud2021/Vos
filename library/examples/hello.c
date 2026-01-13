/**
 * @file hello.c
 * @brief Simple VOS application example
 */

#include <library/Vos.h>

int main(void) {
    vos_info("================================");
    vos_info("Welcome to VOS!");
    vos_info("================================");
    
    // Get system information
    vos_sysinfo_t sysinfo;
    if (vos_sysinfo(&sysinfo) == VOS_OK) {
        vos_info("System Memory:");
        vos_info("  Total:  %u pages", sysinfo.total_memory);
        vos_info("  Free:   %u pages", sysinfo.free_memory);
        vos_info("  Used:   %u pages", sysinfo.used_memory);
        vos_info("  Processes: %u", sysinfo.nr_processes);
    } else {
        vos_err("Failed to get sysinfo");
    }
    
    // Get process info
    vos_pid_t pid = vos_getpid();
    vos_info("Current Process ID: %d", pid);
    
    // Get kernel version
    const char *version = vos_kernel_version();
    vos_info("Kernel Version: %s", version);
    
    vos_info("================================");
    vos_info("Application complete!");
    vos_info("================================");
    
    return 0;
}
