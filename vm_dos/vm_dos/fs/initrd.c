#include "initrd.h"
#include "vfs.h"
#include "kernel/console.h"

static int hello_read(vnode_t *vn, void *buf, int len, int off)
{
    const char *s = (const char *)vn->data;
    int n = 0;
    while (s[off + n] && n < len)
    {
        ((char *)buf)[n] = s[off + n];
        n++;
    }
    return n;
}

void initrd_mount(void)
{
    static const char msg[] = "Hello from initrd!\n";
    static vnode_t vn = {.name = "/hello.txt", .read = hello_read, .data = (void *)msg, .next = 0};
    vfs_add(&vn);
    console_write("[initrd] mounted /hello.txt\n");
}
