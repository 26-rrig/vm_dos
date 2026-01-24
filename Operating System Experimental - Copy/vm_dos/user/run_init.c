#include <stdint.h>
#include "kernel/console.h"
#include "fs/vfs.h"

void run_init(void)
{
    console_write("\n[user] reading /hello.txt -> ");
    char buf[64];
    int n = vfs_read("/hello.txt", buf, sizeof(buf) - 1, 0);
    if (n > 0)
    {
        buf[n] = 0;
        console_write(buf);
    }
    else
    {
        console_write("(not found)\n");
    }
    console_write("\n[user] ready.\n");
}
