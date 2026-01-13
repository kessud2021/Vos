# VOS Library Integration Guide

## Overview

The VOS Library (`library/Vos.h`) is the primary interface for developing applications on the VOS operating system. This single header file provides access to all major system functionality without requiring complex multi-file includes.

## Single File Design

Unlike traditional operating system libraries that span multiple headers, VOS provides:

- **One Public Header**: `Vos.h` - includes all public APIs
- **One Implementation**: `Vos.c` - wraps kernel functions
- **Minimal Dependencies**: Only standard C types
- **Easy Integration**: Just include one file in your application

## Quick Start

### 1. Include the Header

```c
#include <library/Vos.h>

int main(void) {
    vos_info("Hello from VOS!");
    return 0;
}
```

### 2. Compile and Link

```bash
gcc -I/path/to/vos/library myapp.c -o myapp -lvos
```

### 3. Run

```bash
./myapp
```

## API Categories

### Process Management (`vos_*`)
- `vos_fork()` - Create child process
- `vos_exec()` - Execute program
- `vos_exit()` - Terminate process
- `vos_waitpid()` - Wait for child
- `vos_getpid()` / `vos_getppid()` - Get process IDs
- `vos_yield()` - Yield CPU

### Memory Management
- `vos_malloc()` - Allocate kernel memory
- `vos_free()` - Free memory
- `vos_slab_alloc()` / `vos_slab_free()` - Slab cache operations

### File System Operations
- `vos_open()` - Open file
- `vos_read()` / `vos_write()` - File I/O
- `vos_stat()` / `vos_fstat()` - File metadata
- `vos_mkdir()` / `vos_rmdir()` - Directory operations
- `vos_mount()` / `vos_umount()` - Filesystem mounting

### Synchronization
- `vos_spin_lock()` / `vos_spin_unlock()` - Spinlocks
- `vos_sem_init()` / `vos_sem_wait()` / `vos_sem_post()` - Semaphores

### IPC
- `vos_pipe()` - Create pipe
- `vos_pipe_read()` / `vos_pipe_write()` - Pipe I/O

### Timing
- `vos_clock_gettime()` - Get current time
- `vos_sleep()` - Sleep for seconds
- `vos_nanosleep()` - Sleep with nanosecond precision

### Signals
- `vos_signal()` - Register signal handler
- `vos_kill()` - Send signal to process

### System Information
- `vos_sysinfo()` - Get system statistics
- `vos_kernel_version()` - Get kernel version

### Logging
- `vos_info()` - Info message
- `vos_err()` - Error message
- `vos_debug()` - Debug message
- `vos_panic()` - Panic and halt

## Error Handling

All functions that can fail return error codes or -1:

```c
vos_fd_t fd = vos_open("/file.txt", VOS_O_RDONLY, 0);

if (fd < 0) {
    // fd contains error code
    switch (-fd) {
        case VOS_ENOENT:
            vos_err("File not found");
            break;
        case VOS_EACCES:
            vos_err("Permission denied");
            break;
    }
}
```

Standard error codes:
- `VOS_OK` (0) - Success
- `VOS_ENOMEM` (12) - Out of memory
- `VOS_ENOENT` (2) - No such file or directory
- `VOS_EACCES` (13) - Permission denied
- `VOS_EBUSY` (16) - Resource busy
- `VOS_EINVAL` (22) - Invalid argument
- `VOS_ENOTIMPL` (38) - Not implemented

## Common Patterns

### Process Spawning

```c
vos_pid_t pid = vos_fork();

if (pid == 0) {
    // Child process
    char *argv[] = {"/bin/ls", "/", NULL};
    vos_exec("/bin/ls", argv);
    vos_exit(1);  // Only reached on error
} else if (pid > 0) {
    // Parent process
    int status;
    vos_waitpid(pid, &status);
    vos_info("Child exited with status %d", status);
} else {
    // Error
    vos_err("Fork failed");
}
```

### File Reading

```c
vos_fd_t fd = vos_open("/etc/passwd", VOS_O_RDONLY, 0);
if (fd >= 0) {
    char buffer[1024];
    ssize_t n = vos_read(fd, buffer, sizeof(buffer));
    if (n > 0) {
        // Process buffer...
    }
    vos_close(fd);
}
```

### Synchronization

```c
vos_spinlock_t lock = {0};

// Acquire lock
vos_spin_lock(&lock);
{
    // Critical section
    // shared_resource++;
}
vos_spin_unlock(&lock);
```

### Pipe Communication

```c
vos_fd_t read_fd, write_fd;
vos_pipe(&read_fd, &write_fd);

vos_pid_t pid = vos_fork();
if (pid == 0) {
    // Child: write to pipe
    vos_close(read_fd);
    vos_pipe_write(write_fd, "hello", 5);
    vos_close(write_fd);
    vos_exit(0);
} else {
    // Parent: read from pipe
    vos_close(write_fd);
    char buf[6];
    vos_pipe_read(read_fd, buf, 5);
    buf[5] = '\0';
    vos_info("Received: %s", buf);
    vos_close(read_fd);
}
```

## Compilation Examples

### Simple Application

```bash
gcc -I/path/to/vos/library main.c -o main -lvos
```

### CMake Integration

```cmake
find_library(VOS_LIBRARY NAMES vos HINTS /path/to/vos/build/library)

add_executable(myapp main.c)
target_link_libraries(myapp ${VOS_LIBRARY})
target_include_directories(myapp PRIVATE /path/to/vos/library)
```

### With Multiple Source Files

```bash
gcc -I/path/to/vos/library -c file1.c -o file1.o
gcc -I/path/to/vos/library -c file2.c -o file2.o
gcc file1.o file2.o -o myapp -lvos
```

## Platform-Specific Notes

### Architecture Detection

The library automatically detects the target architecture:
- x86_64: `VOS_ARCH_X86_64`
- ARM64: `VOS_ARCH_ARM64`
- RISC-V: `VOS_ARCH_RISCV64`

Architecture-specific code is isolated and transparent to the application.

## Debugging

Enable debug logging by compiling with `-DDEBUG`:

```bash
gcc -DDEBUG -I/path/to/vos/library main.c -o main -lvos
```

Then use `vos_debug()` in your code:

```c
vos_debug("Debug message: x=%d", x);
```

## Performance Considerations

1. **File I/O**: Minimize system calls by using larger buffers
2. **Process Creation**: Fork/exec is expensive; reuse processes when possible
3. **Locking**: Avoid holding spinlocks for extended periods
4. **Memory**: Preallocate where possible to avoid malloc overhead

## Examples

See the `examples/` directory for:
- `hello.c` - Basic VOS application
- `process.c` - Process management
- `fileio.c` - File I/O operations

## Extending the Library

To add new functionality:

1. Add declaration to `Vos.h` (in appropriate section)
2. Add implementation to `Vos.c`
3. Link against kernel functions as needed
4. Document with Doxygen comments
5. Update this guide

## Troubleshooting

### "Undefined reference to vos_*"

Ensure you're linking against libvos:
```bash
gcc ... -lvos  # or link /path/to/libvos.a
```

### "File not found" when opening files

Check the path is correct and use absolute paths:
```c
vos_fd_t fd = vos_open("/tmp/file.txt", VOS_O_RDONLY, 0);
```

### Permission denied

Ensure the process has appropriate permissions:
```c
// Create file with 0666 (rw-rw-rw-)
vos_open("/tmp/file.txt", VOS_O_CREAT | VOS_O_WRONLY, 0666);
```

## See Also

- [Vos.h API Documentation](Vos.h)
- [VOS Kernel Documentation](../kernel/README.md)
- [Build Instructions](../README.md)

## License

VOS Library is part of the VOS Operating System project.
