#ifndef vm_dos_VFS_H
#define vm_dos_VFS_H
#include <stdint.h>
typedef struct vnode vnode_t;
typedef int (*read_fn)(vnode_t *, void *, int, int);
struct vnode
{
    const char *name;
    read_fn read;
    void *data;
    vnode_t *next;
};
void vfs_init(void);
void vfs_add(vnode_t *vn);
vnode_t *vfs_lookup(const char *path);
int vfs_read(const char *path, void *buf, int len, int off);
#endif
#pragma once
void vfs_list(void);
void vfs_cat(const char *path);
