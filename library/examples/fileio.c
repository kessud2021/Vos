/**
 * @file fileio.c
 * @brief VOS file I/O example
 */

#include <library/Vos.h>

int main(void) {
    vos_info("File I/O Example");
    vos_info("================");
    
    const char *path = "/tmp/vos_test.txt";
    const char *data = "Hello, VOS!\n";
    
    // Create and write to file
    vos_info("Opening file: %s", path);
    vos_fd_t fd = vos_open(path, VOS_O_WRONLY | VOS_O_CREAT, 0644);
    
    if (fd >= 0) {
        vos_info("Successfully opened file (fd=%d)", fd);
        
        ssize_t bytes_written = vos_write(fd, data, 12);
        vos_info("Wrote %ld bytes to file", bytes_written);
        
        vos_close(fd);
        vos_info("File closed");
    } else {
        vos_err("Failed to open file (error: %d)", fd);
    }
    
    // Get file stats
    vos_stat_t stat;
    if (vos_stat(path, &stat) == VOS_OK) {
        vos_info("File Info:");
        vos_info("  Inode: %u", stat.ino);
        vos_info("  Size: %u bytes", stat.size);
        vos_info("  Mode: %04x", stat.mode);
        vos_info("  UID: %u", stat.uid);
        vos_info("  GID: %u", stat.gid);
    }
    
    vos_info("Example complete");
    return 0;
}
