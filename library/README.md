# VOS Integration Library

Unified C library for application development on the VOS operating system.

## Overview

`Vos.h` is a single comprehensive header file that provides access to all major VOS system functions. It serves as the primary interface for applications running on VOS, abstracting kernel functionality into clean, documented APIs.

## Features

### Process Management
- Fork, exec, exit, waitpid
- Process information queries
- Priority and scheduling control

### Memory Management
- Kernel memory allocation (malloc/free)
- Slab cache allocation
- Multiple allocation flags (kernel, atomic, user)

### File System
- File operations (open, close, read, write, seek)
- Directory operations (mkdir, rmdir, chdir)
- File metadata (stat, fstat)
- Filesystem mounting/unmounting

### Synchronization
- Spinlocks
- Semaphores
- Mutexes (placeholder)

### Inter-Process Communication
- Pipes
- Pipe read/write operations

### Time & Clocking
- System time queries
- Sleep functions
- Nanosecond precision timers

### Signal Handling
- Signal registration
- Process signaling
- Standard signals (SIGTERM, SIGKILL, SIGINT, etc.)

### System Information
- Memory statistics
- Process count
- CPU information
- System uptime

### Logging
- Info, error, debug logging
- Panic/halt function

## Usage

### Basic Application

```c
#include <library/Vos.h>

int main(void) {
    vos_info("Hello from VOS!");
    
    vos_sysinfo_t sysinfo;
    if (vos_sysinfo(&sysinfo) == VOS_OK) {
        vos_info("Free memory: %u pages", sysinfo.free_memory);
    }
    
    return 0;
}
```

### File I/O

```c
#include <library/Vos.h>

int main(void) {
    // Create and write to file
    vos_fd_t fd = vos_open("/tmp/test.txt", 
                           VOS_O_WRONLY | VOS_O_CREAT, 
                           0644);
    if (fd >= 0) {
        vos_write(fd, "Hello VOS!", 10);
        vos_close(fd);
    }
    
    return 0;
}
```

### Process Management

```c
#include <library/Vos.h>

int main(void) {
    vos_pid_t pid = vos_fork();
    
    if (pid == 0) {
        // Child process
        vos_info("I am child");
        vos_exit(0);
    } else {
        // Parent process
        vos_info("Child PID: %d", pid);
        int status = 0;
        vos_waitpid(pid, &status);
    }
    
    return 0;
}
```

### Synchronization

```c
#include <library/Vos.h>

vos_spinlock_t lock = {0};

void critical_section(void) {
    vos_spin_lock(&lock);
    // Protected code
    vos_spin_unlock(&lock);
}
```

## Error Handling

All VOS functions that can fail return error codes:

```c
int result = vos_open("/file.txt", VOS_O_RDONLY, 0);

if (result < 0) {
    switch (-result) {
        case VOS_ENOENT:
            vos_err("File not found");
            break;
        case VOS_EACCES:
            vos_err("Permission denied");
            break;
        case VOS_ENOMEM:
            vos_err("Out of memory");
            break;
        default:
            vos_err("Unknown error: %d", -result);
    }
}
```

## Building

### CMake Integration

```cmake
include_directories(${VOS_ROOT}/library)

add_executable(my_app main.c)
target_link_libraries(my_app PRIVATE vos)
```

### Direct Compilation

```bash
gcc -I/path/to/vos/library main.c /path/to/vos/library/Vos.c -o my_app
```

## Architecture Support

The library automatically detects and supports:
- x86_64
- ARM64 (aarch64)
- RISC-V 64-bit

Architecture-specific code is isolated and can be extended without modifying the public API.

## API Organization

The header is organized into logical sections with Doxygen documentation:

- **Logging**: Info, error, debug, and panic functions
- **Error Codes**: Standard error constants
- **Type Definitions**: PID, FD, and other fundamental types
- **Memory Management**: Allocation and deallocation
- **Process Management**: Fork, exec, exit, process queries
- **File System**: File and directory operations
- **Synchronization**: Locks and semaphores
- **IPC**: Pipes and message passing
- **Time**: Clock and sleep functions
- **Signals**: Signal handling
- **System Info**: System status queries
- **Utilities**: Macros for common operations

## Implementation Status

- ✅ Complete header with all declarations
- ✅ Stub implementation with kernel integration points
- ⏳ Full system call implementations (ongoing)
- ⏳ Error handling and validation (ongoing)
- ⏳ Performance optimizations (future)

## Design Philosophy

1. **Single File Library**: `Vos.h` provides everything needed
2. **C Compatible**: Works with C and C++ code
3. **Clean API**: Simple, intuitive function names
4. **Well Documented**: Full Doxygen documentation
5. **Extensible**: Easy to add new functionality
6. **Kernel Integration**: Thin wrapper around kernel services

## Future Enhancements

- [ ] Thread management APIs
- [ ] Advanced IPC (message queues, shared memory)
- [ ] Memory mapping (mmap)
- [ ] Device I/O operations
- [ ] Network sockets
- [ ] Performance monitoring

## License

VOS Library is part of the VOS Operating System project.

## Author

VOS Development Team
