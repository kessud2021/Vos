/**
 * Memory management
 *
 * Page allocator, slab allocator, virtual memory
 */

#include <kernel.h>

void *kmalloc(size_t size, gfp_flags_t flags) {
    /* TODO: Implement kmalloc */
    return NULL;
}

void kfree(void *ptr) {
    /* TODO: Implement kfree */
}

int page_allocator_init(void) {
    /* TODO: Initialize page allocator */
    return 0;
}

int slab_allocator_init(void) {
    /* TODO: Initialize slab allocator */
    return 0;
}

int mmap_init(void) {
    /* TODO: Initialize memory mapping */
    return 0;
}
