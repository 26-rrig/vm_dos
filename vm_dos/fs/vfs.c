#include "vfs.h"
#include "kernel/console.h"

static vnode_t *head = 0;

void vfs_init(void)
{
    head = 0;
    console_write("[vfs] init\n");
}
void vfs_add(vnode_t *vn)
{
    vn->next = head;
    head = vn;
}
vnode_t *vfs_lookup(const char *path)
{
    for (vnode_t *p = head; p; p = p->next)
    { /* trivial name match */
        const char *a = p->name, *b = path;
        int ok = 1;
        while (*a || *b)
        {
            if (*a != *b)
            {
                ok = 0;
                break;
            }
            a++;
            b++;
        }
        if (ok)
            return p;
    }
    return 0;
}
int vfs_read(const char *path, void *buf, int len, int off)
{
    vnode_t *vn = vfs_lookup(path);
    if (!vn || !vn->read)
        return -1;
    return vn->read(vn, buf, len, off);
}

#include "ui/console.h"
#include "fs/vfs.h"

void vfs_list(void)
{
    con_write("[vfs] list not implemented\n");
}

void vfs_cat(const char *path)
{
    con_write("[vfs] cat not implemented: ");
    con_write(path);
    con_write("\n");
}
