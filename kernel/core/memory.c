/**
 * Memory Management - Production Grade
 * 
 * Buddy allocator + slab allocator
 */

#include <kernel.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PAGE_SIZE 4096
#define MAX_ORDER 10
#define PHYS_MEM_SIZE (256 * 1024 * 1024)  /* 256 MB */

/* Buddy allocator */
typedef struct free_block {
    struct free_block *next;
    unsigned int order;
} free_block_t;

typedef struct {
    free_block_t *free_lists[MAX_ORDER];
    void *mem_pool;
    size_t mem_size;
} buddy_allocator_t;

static buddy_allocator_t g_buddy;

/* Initialize buddy allocator */
int page_allocator_init(void) {
    /* Allocate 256MB memory pool */
    g_buddy.mem_pool = malloc(PHYS_MEM_SIZE);
    if (!g_buddy.mem_pool) return -ENOMEM;
    
    g_buddy.mem_size = PHYS_MEM_SIZE;
    
    /* Initialize all free lists */
    for (int i = 0; i < MAX_ORDER; i++) {
        g_buddy.free_lists[i] = NULL;
    }
    
    /* Add entire memory as one large block */
    free_block_t *block = (free_block_t *)g_buddy.mem_pool;
    block->order = MAX_ORDER - 1;
    block->next = NULL;
    g_buddy.free_lists[MAX_ORDER - 1] = block;
    
    return 0;
}

/* Allocate pages */
struct page *page_alloc(unsigned int order) {
    if (order >= MAX_ORDER) return NULL;
    
    /* Find suitable block */
    int current_order = order;
    free_block_t *block = NULL;
    
    while (current_order < MAX_ORDER && !block) {
        block = g_buddy.free_lists[current_order];
        if (block) {
            g_buddy.free_lists[current_order] = block->next;
            break;
        }
        current_order++;
    }
    
    if (!block) return NULL;
    
    /* Split blocks down to requested order */
    while (current_order > order) {
        current_order--;
        size_t block_size = 1 << (current_order + PAGE_SHIFT);
        free_block_t *buddy = (free_block_t *)((char *)block + block_size);
        buddy->order = current_order;
        buddy->next = g_buddy.free_lists[current_order];
        g_buddy.free_lists[current_order] = buddy;
        block->order = current_order;
    }
    
    return (struct page *)block;
}

/* Free pages */
void page_free(struct page *page) {
    if (!page) return;
    
    free_block_t *block = (free_block_t *)page;
    unsigned int order = block->order;
    
    /* Coalesce buddies */
    while (order < MAX_ORDER - 1) {
        size_t block_size = 1 << (order + PAGE_SHIFT);
        uintptr_t block_addr = (uintptr_t)block;
        uintptr_t buddy_addr = block_addr ^ block_size;
        free_block_t *buddy = (free_block_t *)buddy_addr;
        
        /* Check if buddy is free */
        if (!buddy || buddy->order != order) break;
        
        /* Remove buddy from free list */
        free_block_t **list = &g_buddy.free_lists[order];
        while (*list && *list != buddy) list = &(*list)->next;
        if (*list) *list = buddy->next;
        
        /* Merge */
        if (buddy_addr < block_addr) {
            block = buddy;
        }
        block->order = ++order;
    }
    
    /* Add back to free list */
    block->next = g_buddy.free_lists[order];
    g_buddy.free_lists[order] = block;
}

void *page_to_virt(struct page *page) {
    return (void *)page;
}

struct page *virt_to_page(void *vaddr) {
    return (struct page *)vaddr;
}

/* Slab allocator */
#define SLAB_SIZE 4096
typedef struct slab {
    void *pool;
    size_t item_size;
    unsigned char bitmap[512];  /* 4096 items max */
    int num_free;
} slab_t;

static slab_t g_slabs[16];
static int g_num_slabs = 0;

int slab_allocator_init(void) {
    g_num_slabs = 0;
    return 0;
}

/* kmalloc - allocate kernel memory */
void *kmalloc(size_t size, gfp_flags_t flags) {
    if (size == 0 || size > SLAB_SIZE) return NULL;
    
    /* Find or create slab */
    for (int i = 0; i < g_num_slabs; i++) {
        if (g_slabs[i].item_size >= size && g_slabs[i].num_free > 0) {
            /* Find free slot */
            for (int j = 0; j < (SLAB_SIZE / g_slabs[i].item_size); j++) {
                if (!g_slabs[i].bitmap[j / 8] & (1 << (j % 8))) {
                    g_slabs[i].bitmap[j / 8] |= (1 << (j % 8));
                    g_slabs[i].num_free--;
                    return (char *)g_slabs[i].pool + (j * g_slabs[i].item_size);
                }
            }
        }
    }
    
    /* Create new slab */
    if (g_num_slabs >= 16) return NULL;
    
    slab_t *slab = &g_slabs[g_num_slabs];
    slab->pool = malloc(SLAB_SIZE);
    if (!slab->pool) return NULL;
    
    slab->item_size = size;
    slab->num_free = SLAB_SIZE / size;
    memset(slab->bitmap, 0, sizeof(slab->bitmap));
    
    g_num_slabs++;
    
    /* Allocate first item */
    slab->bitmap[0] = 1;
    slab->num_free--;
    return slab->pool;
}

/* kfree - free kernel memory */
void kfree(void *ptr) {
    if (!ptr) return;
    
    for (int i = 0; i < g_num_slabs; i++) {
        if ((char *)ptr >= (char *)g_slabs[i].pool &&
            (char *)ptr < (char *)g_slabs[i].pool + SLAB_SIZE) {
            
            size_t offset = (char *)ptr - (char *)g_slabs[i].pool;
            size_t idx = offset / g_slabs[i].item_size;
            g_slabs[i].bitmap[idx / 8] &= ~(1 << (idx % 8));
            g_slabs[i].num_free++;
            return;
        }
    }
}

int mmap_init(void) {
    return 0;  /* Virtual memory mapping - stub for now */
}
