#include <stdint.h>
#include "vfs.h"

typedef struct
{
    const uint8_t *d;
    int sz;
} blob_t;

static int r_read(vnode_t *vn, void *buf, int len, int off)
{
    blob_t *b = (blob_t *)vn->data;
    if (off >= b->sz)
        return 0;
    int n = (off + len > b->sz) ? (b->sz - off) : len;
    for (int i = 0; i < n; i++)
        ((uint8_t *)buf)[i] = b->d[off + i];
    return n;
}

void ramfs_add(const char *name, const void *data, int size)
{
    static blob_t blobs[32];
    static int bi = 0;
    blob_t *b = &blobs[bi++];
    b->d = (const uint8_t *)data;
    b->sz = size;
    static vnode_t nodes[32];
    static int ni = 0;
    vnode_t *vn = &nodes[ni++];
    vn->name = name;
    vn->read = r_read;
    vn->data = b;
    vn->next = 0;
    vfs_add(vn);
}