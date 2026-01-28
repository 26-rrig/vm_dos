#include "libc/std.h"
#include "libc/io.h"

void user_init_start(void)
{
    write("\nvm_dos userland online\n");
    write("Launching shell...\n");
    extern void shell_main(void);
    shell_main();
}