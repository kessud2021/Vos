/**
 * @file Vos.h
 * @brief VOS Operating System Integration Library
 * 
 * Unified header for application development on the VOS operating system.
 * Provides access to core OS functionality including process management,
 * memory allocation, file system operations, IPC, and synchronization primitives.
 * 
 * @version 1.0
 * @author VOS Development Team
 */

#ifndef __VOS_H__
#define __VOS_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Standard type definitions (may be predefined in system headers) */
#ifndef __off_t_defined
typedef long off_t;
#define __off_t_defined
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Architecture Detection & Definitions
 * ============================================================================ */

#ifdef __x86_64__
    #define VOS_ARCH_X86_64
#elif defined(__aarch64__)
    #define VOS_ARCH_ARM64
#elif defined(__riscv) && __riscv_xlen == 64
    #define VOS_ARCH_RISCV64
#else
    #define VOS_ARCH_UNKNOWN
#endif

#define VOS_VERSION_MAJOR   0
#define VOS_VERSION_MINOR   1
#define VOS_VERSION_PATCH   0

/* ============================================================================
 * Logging & Debug Output
 * ============================================================================ */

/**
 * @defgroup Logging Logging Functions
 * @{
 */

/**
 * Log an informational message
 * @param fmt Format string (printf-style)
 */
void vos_info(const char *fmt, ...);

/**
 * Log an error message
 * @param fmt Format string (printf-style)
 */
void vos_err(const char *fmt, ...);

/**
 * Log a debug message (only in debug builds)
 * @param fmt Format string (printf-style)
 */
void vos_debug(const char *fmt, ...);

/**
 * Panic and halt the system
 * @param fmt Format string (printf-style)
 */
void vos_panic(const char *fmt, ...);

/** @} */

/* ============================================================================
 * Error Codes
 * ============================================================================ */

#define VOS_OK              0       /**< Success */
#define VOS_ENOMEM          12      /**< Out of memory */
#define VOS_ENOENT          2       /**< No such file or directory */
#define VOS_EACCES          13      /**< Permission denied */
#define VOS_EBUSY           16      /**< Device or resource busy */
#define VOS_EINVAL          22      /**< Invalid argument */
#define VOS_ENOTIMPL        38      /**< Function not implemented */
#define VOS_EBADF           9       /**< Bad file descriptor */
#define VOS_EAGAIN          11      /**< Try again */

/* ============================================================================
 * Type Definitions
 * ============================================================================ */

typedef int32_t vos_pid_t;         /**< Process ID */
typedef int32_t vos_tid_t;         /**< Thread ID */
typedef int32_t vos_fd_t;          /**< File descriptor */

#define VOS_PID_MAX         32768
#define VOS_INIT_PID        1
#define VOS_INVALID_PID     -1
#define VOS_INVALID_FD      -1

/* Memory size constants */
#define VOS_PAGE_SHIFT      12
#define VOS_PAGE_SIZE       (1UL << VOS_PAGE_SHIFT)
#define VOS_PAGE_MASK       (~(VOS_PAGE_SIZE - 1))

/* ============================================================================
 * Memory Management
 * ============================================================================ */

/**
 * @defgroup Memory Memory Management
 * @{
 */

typedef enum {
    VOS_GFP_KERNEL = 0,    /**< Kernel allocation (may sleep) */
    VOS_GFP_ATOMIC = 1,    /**< Atomic allocation (cannot sleep) */
    VOS_GFP_USER = 2,      /**< User-space allocation */
} vos_gfp_flags_t;

/**
 * Allocate kernel memory
 * @param size Number of bytes to allocate
 * @param flags Allocation flags
 * @return Pointer to allocated memory, or NULL on failure
 */
void *vos_malloc(size_t size, vos_gfp_flags_t flags);

/**
 * Free allocated memory
 * @param ptr Pointer to memory to free
 */
void vos_free(void *ptr);

/**
 * Allocate contiguous memory from slab cache
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory, or NULL on failure
 */
void *vos_slab_alloc(size_t size);

/**
 * Free slab-allocated memory
 * @param ptr Pointer to memory to free
 * @param size Size of the allocation
 */
void vos_slab_free(void *ptr, size_t size);

/** @} */

/* ============================================================================
 * Process Management
 * ============================================================================ */

/**
 * @defgroup Process Process Management
 * @{
 */

typedef enum {
    VOS_TASK_RUNNABLE,
    VOS_TASK_INTERRUPTIBLE,
    VOS_TASK_UNINTERRUPTIBLE,
    VOS_TASK_STOPPED,
    VOS_TASK_TRACED,
    VOS_TASK_DEAD,
} vos_task_state_t;

typedef struct {
    vos_pid_t pid;              /**< Process ID */
    vos_pid_t ppid;             /**< Parent process ID */
    vos_task_state_t state;     /**< Task state */
    int priority;               /**< Priority (-20 to +19) */
    uint64_t vruntime;          /**< Virtual runtime */
} vos_process_info_t;

/**
 * Fork the current process
 * @return Child PID in parent, 0 in child, or error code
 */
vos_pid_t vos_fork(void);

/**
 * Execute a program
 * @param filename Path to executable
 * @param argv Command-line arguments
 * @return Error code (never returns on success)
 */
int vos_exec(const char *filename, char *const argv[]);

/**
 * Exit the current process
 * @param code Exit status code
 */
void vos_exit(int code) __attribute__((noreturn));

/**
 * Wait for child process
 * @param pid Process ID to wait for (-1 for any)
 * @param status Pointer to store exit status
 * @return PID of reaped child, or error code
 */
vos_pid_t vos_waitpid(vos_pid_t pid, int *status);

/**
 * Get current process ID
 * @return Current PID
 */
vos_pid_t vos_getpid(void);

/**
 * Get parent process ID
 * @return Parent PID
 */
vos_pid_t vos_getppid(void);

/**
 * Get process information
 * @param pid Process ID
 * @param info Pointer to process info structure
 * @return Error code
 */
int vos_get_process_info(vos_pid_t pid, vos_process_info_t *info);

/**
 * Yield CPU to next process
 */
void vos_yield(void);

/** @} */

/* ============================================================================
 * File System Operations
 * ============================================================================ */

/**
 * @defgroup FileSystem File System Operations
 * @{
 */

#define VOS_O_RDONLY        0x00
#define VOS_O_WRONLY        0x01
#define VOS_O_RDWR          0x02
#define VOS_O_APPEND        0x08
#define VOS_O_CREAT         0x100
#define VOS_O_EXCL          0x200
#define VOS_O_TRUNC         0x1000

typedef struct {
    uint32_t ino;               /**< Inode number */
    uint32_t size;              /**< File size in bytes */
    uint16_t mode;              /**< File mode (type + permissions) */
    uint32_t uid;               /**< Owner user ID */
    uint32_t gid;               /**< Owner group ID */
    uint32_t atime;             /**< Access time */
    uint32_t mtime;             /**< Modification time */
    uint32_t ctime;             /**< Change time */
} vos_stat_t;

typedef struct {
    char name[256];
    uint32_t ino;
    uint16_t mode;
} vos_dirent_t;

/**
 * Open a file
 * @param path File path
 * @param flags Open flags
 * @param mode File mode (for creation)
 * @return File descriptor or error code
 */
vos_fd_t vos_open(const char *path, int flags, int mode);

/**
 * Close a file
 * @param fd File descriptor
 * @return Error code
 */
int vos_close(vos_fd_t fd);

/**
 * Read from file
 * @param fd File descriptor
 * @param buf Buffer to read into
 * @param count Number of bytes to read
 * @return Number of bytes read or error code
 */
ssize_t vos_read(vos_fd_t fd, void *buf, size_t count);

/**
 * Write to file
 * @param fd File descriptor
 * @param buf Buffer to write
 * @param count Number of bytes to write
 * @return Number of bytes written or error code
 */
ssize_t vos_write(vos_fd_t fd, const void *buf, size_t count);

/**
 * Seek in file
 * @param fd File descriptor
 * @param offset Offset to seek to
 * @param whence Seek origin (0=start, 1=current, 2=end)
 * @return New file position or error code
 */
off_t vos_lseek(vos_fd_t fd, off_t offset, int whence);

/**
 * Get file status
 * @param path File path
 * @param stat Pointer to stat structure
 * @return Error code
 */
int vos_stat(const char *path, vos_stat_t *stat);

/**
 * Get file descriptor status
 * @param fd File descriptor
 * @param stat Pointer to stat structure
 * @return Error code
 */
int vos_fstat(vos_fd_t fd, vos_stat_t *stat);

/**
 * Create a directory
 * @param path Directory path
 * @param mode Directory mode
 * @return Error code
 */
int vos_mkdir(const char *path, int mode);

/**
 * Remove a file
 * @param path File path
 * @return Error code
 */
int vos_unlink(const char *path);

/**
 * Remove a directory
 * @param path Directory path
 * @return Error code
 */
int vos_rmdir(const char *path);

/**
 * Change working directory
 * @param path Directory path
 * @return Error code
 */
int vos_chdir(const char *path);

/**
 * Get current working directory
 * @param buf Buffer for path
 * @param size Buffer size
 * @return Pointer to path or NULL on error
 */
char *vos_getcwd(char *buf, size_t size);

/**
 * Mount a filesystem
 * @param device Device path
 * @param mount_point Mount point path
 * @param fs_type Filesystem type (e.g., "ext4", "tmpfs")
 * @return Error code
 */
int vos_mount(const char *device, const char *mount_point, const char *fs_type);

/**
 * Unmount a filesystem
 * @param mount_point Mount point path
 * @return Error code
 */
int vos_umount(const char *mount_point);

/** @} */

/* ============================================================================
 * Synchronization Primitives
 * ============================================================================ */

/**
 * @defgroup Sync Synchronization Primitives
 * @{
 */

typedef struct {
    volatile int val;
} vos_spinlock_t;

typedef struct {
    volatile int count;
} vos_semaphore_t;

typedef struct {
    volatile int val;
} vos_mutex_t;

/**
 * Acquire spinlock
 * @param lock Spinlock pointer
 */
void vos_spin_lock(vos_spinlock_t *lock);

/**
 * Release spinlock
 * @param lock Spinlock pointer
 */
void vos_spin_unlock(vos_spinlock_t *lock);

/**
 * Try to acquire spinlock
 * @param lock Spinlock pointer
 * @return 1 if acquired, 0 if already locked
 */
int vos_spin_trylock(vos_spinlock_t *lock);

/**
 * Initialize semaphore
 * @param sem Semaphore pointer
 * @param value Initial value
 * @return Error code
 */
int vos_sem_init(vos_semaphore_t *sem, int value);

/**
 * Wait on semaphore (decrement)
 * @param sem Semaphore pointer
 */
void vos_sem_wait(vos_semaphore_t *sem);

/**
 * Signal semaphore (increment)
 * @param sem Semaphore pointer
 */
void vos_sem_post(vos_semaphore_t *sem);

/** @} */

/* ============================================================================
 * Inter-Process Communication (IPC)
 * ============================================================================ */

/**
 * @defgroup IPC Inter-Process Communication
 * @{
 */

/**
 * Create a pipe
 * @param read_fd Pointer to store read end FD
 * @param write_fd Pointer to store write end FD
 * @return Error code
 */
int vos_pipe(vos_fd_t *read_fd, vos_fd_t *write_fd);

/**
 * Write to pipe
 * @param fd Pipe file descriptor
 * @param buf Data to write
 * @param len Number of bytes
 * @return Bytes written or error code
 */
ssize_t vos_pipe_write(vos_fd_t fd, const void *buf, size_t len);

/**
 * Read from pipe
 * @param fd Pipe file descriptor
 * @param buf Buffer to read into
 * @param len Buffer size
 * @return Bytes read or error code
 */
ssize_t vos_pipe_read(vos_fd_t fd, void *buf, size_t len);

/** @} */

/* ============================================================================
 * Time & Clock
 * ============================================================================ */

/**
 * @defgroup Time Time & Clock Functions
 * @{
 */

typedef struct {
    uint32_t sec;   /**< Seconds */
    uint32_t nsec;  /**< Nanoseconds */
} vos_timespec_t;

/**
 * Get monotonic clock time
 * @param ts Pointer to timespec structure
 * @return Error code
 */
int vos_clock_gettime(vos_timespec_t *ts);

/**
 * Sleep for specified time
 * @param seconds Seconds to sleep
 * @return Error code
 */
int vos_sleep(uint32_t seconds);

/**
 * Sleep for nanoseconds
 * @param ts Pointer to timespec (duration)
 * @return Error code
 */
int vos_nanosleep(const vos_timespec_t *ts);

/** @} */

/* ============================================================================
 * Signal Handling
 * ============================================================================ */

/**
 * @defgroup Signals Signal Handling
 * @{
 */

typedef void (*vos_signal_handler_t)(int sig);

#define VOS_SIGTERM 15      /**< Termination signal */
#define VOS_SIGKILL 9       /**< Kill signal */
#define VOS_SIGINT  2       /**< Interrupt signal */
#define VOS_SIGSEGV 11      /**< Segmentation fault */
#define VOS_SIGABRT 6       /**< Abort signal */

/**
 * Register signal handler
 * @param sig Signal number
 * @param handler Handler function
 * @return Previous handler or error
 */
vos_signal_handler_t vos_signal(int sig, vos_signal_handler_t handler);

/**
 * Send signal to process
 * @param pid Process ID
 * @param sig Signal number
 * @return Error code
 */
int vos_kill(vos_pid_t pid, int sig);

/** @} */

/* ============================================================================
 * System Information
 * ============================================================================ */

/**
 * @defgroup SysInfo System Information
 * @{
 */

typedef struct {
    uint32_t total_memory;      /**< Total system memory in pages */
    uint32_t free_memory;       /**< Free memory in pages */
    uint32_t used_memory;       /**< Used memory in pages */
    uint32_t nr_processes;      /**< Number of processes */
    uint32_t uptime;            /**< System uptime in seconds */
} vos_sysinfo_t;

/**
 * Get system information
 * @param info Pointer to sysinfo structure
 * @return Error code
 */
int vos_sysinfo(vos_sysinfo_t *info);

/**
 * Get number of CPU cores
 * @return Number of cores
 */
uint32_t vos_sysconf_nprocs(void);

/** @} */

/* ============================================================================
 * Utility Macros
 * ============================================================================ */

/**
 * @defgroup Util Utility Macros
 * @{
 */

#define VOS_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            vos_panic("Assertion failed: %s at %s:%d", msg, __FILE__, __LINE__); \
        } \
    } while(0)

#define VOS_LIKELY(x)       __builtin_expect(!!(x), 1)
#define VOS_UNLIKELY(x)     __builtin_expect(!!(x), 0)

#define VOS_ALIGN_UP(x, align)      (((x) + (align) - 1) & ~((align) - 1))
#define VOS_ALIGN_DOWN(x, align)    ((x) & ~((align) - 1))
#define VOS_IS_ALIGNED(x, align)    (((x) & ((align) - 1)) == 0)

#define VOS_MIN(a, b)       ((a) < (b) ? (a) : (b))
#define VOS_MAX(a, b)       ((a) > (b) ? (a) : (b))

/** @} */

/* ============================================================================
 * Kernel Info
 * ============================================================================ */

/**
 * Get kernel version string
 * @return Version string
 */
const char *vos_kernel_version(void);

/**
 * Check if running on VOS
 * @return 1 if running on VOS, 0 otherwise
 */
int vos_is_vos_kernel(void);

#ifdef __cplusplus
}
#endif

#endif /* __VOS_H__ */
