/**
 * Kernel main header
 *
 * Core definitions and function declarations for the VSS-CO OS kernel.
 */

#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Architecture-specific */
#ifdef __x86_64__
    #include "arch/x86_64.h"
#elif defined(__aarch64__)
    #include "arch/arm64.h"
#elif defined(__riscv) && __riscv_xlen == 64
    #include "arch/riscv64.h"
#endif

/* Kernel printk */
void pr_info(const char *fmt, ...);
void pr_err(const char *fmt, ...);
void pr_debug(const char *fmt, ...);
void pr_panic(const char *fmt, ...);

/* Error codes (kernel) */
#define -ENOMEM     -12
#define -ENOENT     -2
#define -EACCES     -13
#define -EBUSY      -16
#define -EINVAL     -22
#define -ENOTIMPL   -38

/* PID & TID */
typedef int32_t pid_t;
typedef int32_t tid_t;
#define PID_MAX 32768
#define INIT_PID 1

/* Physical/Virtual addresses */
typedef uintptr_t phys_addr_t;
typedef uintptr_t virt_addr_t;

#define PAGE_SHIFT  12
#define PAGE_SIZE   (1UL << PAGE_SHIFT)
#define PAGE_MASK   (~(PAGE_SIZE - 1))

/* Memory allocators */
typedef enum {
    GFP_KERNEL = 0,    /* May sleep */
    GFP_ATOMIC = 1,    /* Cannot sleep */
    GFP_USER = 2,      /* User-allocated */
} gfp_flags_t;

void *kmalloc(size_t size, gfp_flags_t flags);
void kfree(void *ptr);
void *kmem_cache_alloc(size_t size);
void kmem_cache_free(void *ptr, size_t size);

/* Page allocator */
struct page;
struct page *page_alloc(unsigned int order);
void page_free(struct page *page);
void *page_to_virt(struct page *page);
struct page *virt_to_page(void *vaddr);

/* Scheduler */
typedef enum {
    TASK_RUNNABLE,
    TASK_INTERRUPTIBLE,
    TASK_UNINTERRUPTIBLE,
    TASK_STOPPED,
    TASK_TRACED,
    TASK_DEAD,
} task_state_t;

struct task_struct {
    pid_t pid;
    pid_t ppid;
    task_state_t state;
    int priority;       /* -20 (high) to +19 (low) */
    uint64_t vruntime;  /* Virtual runtime */
    void *stack;
    void *mm;           /* Memory management */
    void *files;        /* Open file descriptors */
};

pid_t do_fork(void);
int do_exec(const char *filename, char *const argv[]);
void do_exit(int code);
void schedule(void);

/* Synchronization */
typedef struct {
    volatile int val;
} spinlock_t;

void spin_lock(spinlock_t *lock);
void spin_unlock(spinlock_t *lock);
int spin_trylock(spinlock_t *lock);

typedef struct {
    volatile int count;
} semaphore_t;

int sem_init(semaphore_t *sem, int value);
void sem_wait(semaphore_t *sem);
void sem_post(semaphore_t *sem);

/* IPC */
typedef int pipe_t;
int pipe_create(pipe_t *read_fd, pipe_t *write_fd);
int pipe_write(pipe_t fd, const void *buf, size_t len);
int pipe_read(pipe_t fd, void *buf, size_t len);

/* Virtual File System */
typedef struct inode {
    uint32_t ino;
    uint32_t size;
    uint16_t mode;      /* File type & permissions */
    uint32_t uid, gid;
    uint32_t atime, mtime, ctime;
    void *fs_data;      /* Filesystem-specific */
} inode_t;

typedef struct {
    char name[256];
    inode_t *inode;
} dentry_t;

inode_t *inode_alloc(void);
void inode_free(inode_t *ino);
int inode_read(inode_t *ino, void *buf, size_t count, uint32_t offset);
int inode_write(inode_t *ino, const void *buf, size_t count, uint32_t offset);

/* Filesystem operations */
int mount_fs(const char *device, const char *mount_point, const char *fs_type);
int umount_fs(const char *mount_point);

/* Initialization functions */
void kernel_main(void);
void init_cpu(void);
void init_memory(void);
void init_scheduler(void);
void init_vfs(void);
void init_drivers(void);

#endif /* __KERNEL_H__ */
