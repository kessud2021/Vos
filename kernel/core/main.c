/**
 * Kernel entry point
 *
 * Called by bootloader (GRUB2 / EFI)
 * Initializes all subsystems and starts scheduling
 */

#include <kernel.h>

/* Kernel symbol addresses (from linker script) */
extern uint8_t __kernel_start[];
extern uint8_t __kernel_end[];
extern uint8_t __bss_start[];
extern uint8_t __bss_end[];

/**
 * kernel_main
 *
 * Main kernel entry point. Called after bootloader setup.
 * Sets up all subsystems, mounts root filesystem, and starts init process.
 */
void kernel_main(void) {
    pr_info("VSS-CO OS Kernel v1.0\n");
    pr_info("================================================\n\n");

    /* Initialize architecture */
    pr_info("Initializing CPU...\n");
    init_cpu();
    pr_info("✓ CPU initialized\n\n");

    /* Initialize memory management */
    pr_info("Initializing memory management...\n");
    init_memory();
    pr_info("✓ Memory management initialized\n\n");

    /* Initialize process scheduler */
    pr_info("Initializing scheduler...\n");
    init_scheduler();
    pr_info("✓ Scheduler initialized\n\n");

    /* Initialize virtual filesystem */
    pr_info("Initializing VFS...\n");
    init_vfs();
    pr_info("✓ VFS initialized\n\n");

    /* Initialize device drivers */
    pr_info("Initializing drivers...\n");
    init_drivers();
    pr_info("✓ Drivers initialized\n\n");

    /* Mount root filesystem */
    pr_info("Mounting root filesystem...\n");
    int ret = mount_fs("/dev/root", "/", "ext4");
    if (ret < 0) {
        pr_err("Failed to mount root filesystem\n\n");
        pr_info("Kernel initialization complete (test mode)\n");
        pr_info("================================================\n");
        return;
    }
    pr_info("✓ Root filesystem mounted\n\n");

    /* Create and start init process (PID 1) */
    pr_info("Forking init process...\n");
    pid_t init_pid = do_fork();
    if (init_pid == 0) {
        /* Child process: exec /sbin/init */
        pr_info("Executing /sbin/init...\n");
        char *init_args[] = { "/sbin/init", NULL };
        do_exec("/sbin/init", init_args);
        /* Should not return */
        pr_panic("Failed to exec /sbin/init\n");
    } else if (init_pid > 0) {
        pr_info("✓ Init process started (PID %d)\n\n", init_pid);
    } else {
        pr_err("Failed to fork init process\n\n");
        pr_info("Kernel initialization complete (test mode)\n");
        pr_info("================================================\n");
        return;
    }

    /* Start the scheduler */
    pr_info("Starting scheduler...\n");
    schedule();

    /* Should never reach here */
    pr_panic("Schedule returned unexpectedly\n");
}

/**
 * init_cpu
 *
 * Initialize CPU state:
 * - Setup GDT (Global Descriptor Table)
 * - Setup IDT (Interrupt Descriptor Table)
 * - Setup TSS (Task State Segment)
 * - Setup paging
 * - Enable interrupts
 */
void init_cpu(void) {
    /* Architecture-specific initialization */
    #ifdef __x86_64__
    extern void init_cpu_x86_64(void);
    init_cpu_x86_64();
    #endif
}

/**
 * init_memory
 *
 * Initialize memory management:
 * - Setup page allocator
 * - Setup slab allocator
 * - Setup virtual memory areas (VMAs)
 * - Map kernel memory
 */
void init_memory(void) {
    /* Page allocator setup */
    extern int page_allocator_init(void);
    if (page_allocator_init() < 0) {
        pr_panic("Page allocator initialization failed\n");
    }

    /* Slab allocator setup */
    extern int slab_allocator_init(void);
    if (slab_allocator_init() < 0) {
        pr_panic("Slab allocator initialization failed\n");
    }

    /* Memory mapping setup */
    extern int mmap_init(void);
    if (mmap_init() < 0) {
        pr_panic("Memory mapping initialization failed\n");
    }
}

/**
 * init_scheduler
 *
 * Initialize process scheduler:
 * - Create idle task
 * - Setup run queue
 * - Setup load balancing (if SMP)
 */
void init_scheduler(void) {
    extern int scheduler_init(void);
    if (scheduler_init() < 0) {
        pr_panic("Scheduler initialization failed\n");
    }
}

/**
 * init_vfs
 *
 * Initialize virtual filesystem:
 * - Register filesystem drivers (ext4, tmpfs, procfs, etc.)
 * - Create root mount point
 * - Initialize inode/dentry caches
 */
void init_vfs(void) {
    extern int vfs_init(void);
    if (vfs_init() < 0) {
        pr_panic("VFS initialization failed\n");
    }

    /* Register built-in filesystems */
    extern int register_ext4_fs(void);
    extern int register_tmpfs_fs(void);
    extern int register_procfs_fs(void);

    if (register_ext4_fs() < 0)
        pr_err("Failed to register ext4\n");
    if (register_tmpfs_fs() < 0)
        pr_err("Failed to register tmpfs\n");
    if (register_procfs_fs() < 0)
        pr_err("Failed to register procfs\n");
}

/**
 * init_drivers
 *
 * Initialize device drivers:
 * - Console driver (keyboard, screen)
 * - Block device drivers (disk, USB)
 * - Network drivers (if available)
 * - Other platform devices
 */
void init_drivers(void) {
    extern int console_driver_init(void);
    extern int block_driver_init(void);

    if (console_driver_init() < 0)
        pr_err("Console driver initialization failed\n");

    if (block_driver_init() < 0)
        pr_err("Block driver initialization failed\n");

    /* Detect and initialize PCI devices */
    extern int pci_scan_devices(void);
    pci_scan_devices();
}

/**
 * pr_info, pr_err, pr_debug, pr_panic
 *
 * Kernel logging functions
 * Outputs to console during boot, to syslog later
 * 
 * These are defined in the test harness or bootloader
 */

/* Declared but defined elsewhere */
extern void pr_info(const char *fmt, ...);
extern void pr_err(const char *fmt, ...);
extern void pr_debug(const char *fmt, ...);
extern void pr_panic(const char *fmt, ...);
