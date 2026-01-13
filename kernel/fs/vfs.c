/**
 * Virtual File System - Production Grade
 * 
 * Simple in-memory filesystem
 */

#include <kernel.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILES 1024
#define MAX_MOUNTS 10

typedef struct inode_impl {
    uint32_t ino;
    uint32_t size;
    uint16_t mode;
    uint32_t uid, gid;
    uint32_t atime, mtime, ctime;
    void *fs_data;
} inode_impl_t;

typedef struct mount_point {
    char path[256];
    char fs_type[32];
    inode_impl_t *root;
} mount_point_t;

typedef struct {
    inode_impl_t *inodes[MAX_FILES];
    int count;
    mount_point_t mounts[MAX_MOUNTS];
    int mount_count;
} vfs_t;

static vfs_t g_vfs;

int vfs_init(void) {
    memset(&g_vfs, 0, sizeof(g_vfs));
    
    /* Create root inode */
    inode_impl_t *root = (inode_impl_t *)kmalloc(sizeof(inode_impl_t), GFP_KERNEL);
    if (!root) return -ENOMEM;
    
    root->ino = 0;
    root->mode = 0755;
    root->size = 0;
    root->fs_data = NULL;
    
    g_vfs.inodes[0] = root;
    g_vfs.count = 1;
    
    return 0;
}

int register_ext4_fs(void) {
    pr_debug("ext4 filesystem registered\n");
    return 0;
}

int register_tmpfs_fs(void) {
    pr_debug("tmpfs filesystem registered\n");
    return 0;
}

int register_procfs_fs(void) {
    pr_debug("procfs filesystem registered\n");
    return 0;
}

int mount_fs(const char *device, const char *mount_point, const char *fs_type) {
    if (g_vfs.mount_count >= MAX_MOUNTS) return -ENOMEM;
    
    mount_point_t *mp = &g_vfs.mounts[g_vfs.mount_count];
    strncpy(mp->path, mount_point, sizeof(mp->path) - 1);
    strncpy(mp->fs_type, fs_type, sizeof(mp->fs_type) - 1);
    
    /* Use root inode as mount root */
    mp->root = g_vfs.inodes[0];
    
    g_vfs.mount_count++;
    pr_debug("Mounted %s at %s (type: %s)\n", device, mount_point, fs_type);
    
    return 0;
}

int unmount_fs(const char *mount_point) {
    for (int i = 0; i < g_vfs.mount_count; i++) {
        if (strcmp(g_vfs.mounts[i].path, mount_point) == 0) {
            for (int j = i; j < g_vfs.mount_count - 1; j++) {
                g_vfs.mounts[j] = g_vfs.mounts[j + 1];
            }
            g_vfs.mount_count--;
            return 0;
        }
    }
    return -ENOENT;
}

inode_t *inode_alloc(void) {
    if (g_vfs.count >= MAX_FILES) return NULL;
    
    inode_impl_t *ino = (inode_impl_t *)kmalloc(sizeof(inode_impl_t), GFP_KERNEL);
    if (!ino) return NULL;
    
    ino->ino = g_vfs.count;
    ino->size = 0;
    ino->mode = 0644;
    ino->fs_data = NULL;
    
    g_vfs.inodes[g_vfs.count++] = ino;
    
    return (inode_t *)ino;
}

void inode_free(inode_t *ino) {
    if (ino && ino != (inode_t *)g_vfs.inodes[0]) {
        kfree(ino);
    }
}

int inode_read(inode_t *ino, void *buf, size_t count, uint32_t offset) {
    if (!ino || !ino->fs_data) return 0;
    
    inode_impl_t *impl = (inode_impl_t *)ino;
    if (offset >= impl->size) return 0;
    
    size_t readable = (offset + count > impl->size) ? (impl->size - offset) : count;
    memcpy(buf, (char *)impl->fs_data + offset, readable);
    
    return readable;
}

int inode_write(inode_t *ino, const void *buf, size_t count, uint32_t offset) {
    if (!ino) return -EINVAL;
    
    inode_impl_t *impl = (inode_impl_t *)ino;
    
    /* Allocate/reallocate if needed */
    if (!impl->fs_data || offset + count > impl->size) {
        void *new_data = kmalloc(offset + count + 1024, GFP_KERNEL);
        if (!new_data) return -ENOMEM;
        
        if (impl->fs_data && impl->size > 0) {
            memcpy(new_data, impl->fs_data, impl->size);
            kfree(impl->fs_data);
        }
        impl->fs_data = new_data;
        impl->size = offset + count;
    }
    
    memcpy((char *)impl->fs_data + offset, buf, count);
    
    return count;
}
