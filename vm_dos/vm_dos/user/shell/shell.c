#include "libc/std.h"
#include "libc/io.h"
#include "tui.h"

static void print_num(unsigned long v)
{
    char buf[32];
    int i = 0;
    if (v == 0)
    {
        write("0");
        return;
    }
    char tmp[32];
    int n = 0;
    while (v)
    {
        tmp[n++] = '0' + (v % 10);
        v /= 10;
    }
    for (int j = n - 1; j >= 0; j--)
        buf[i++] = tmp[j];
    buf[i] = 0;
    write(buf);
}

static void exec(const char *cmd)
{
    if (!cmd || !*cmd)
        return;
    if (strcmp(cmd, "help") == 0)
    {
        write("Commands: help, time, echo <text>, clear\n");
    }
    else if (strcmp(cmd, "time") == 0)
    {
        write("ticks: ");
        print_num(ticks());
        write("\n");
    }
    else if (cmd[0] == 'e' && cmd[1] == 'c' && cmd[2] == 'h' && cmd[3] == 'o' && (cmd[4] == ' ' || cmd[4] == 0))
    {
        const char *s = cmd[4] ? cmd + 5 : "";
        write(s);
        write("\n");
    }
    else if (strcmp(cmd, "clear") == 0)
    {
        for (int i = 0; i < 30; i++)
            write("\n");
    }
    else
    {
        write("Unknown: ");
        write(cmd);
        write("\n");
    }
}

void shell_main(void)
{
    write("Welcome to vm_dos\n");
    write("Type 'help' to begin.\n");
    char buf[128];
    for (;;)
    {
        int n = 0;
        tui_prompt();
        for (;;)
        {
            int got = read(&buf[n], 1);
            if (got == 1)
            {
                char ch = buf[n];
                if (ch == '\n')
                {
                    buf[n] = 0;
                    break;
                }
                write((char[]){ch, 0});
                if (++n == (int)sizeof(buf) - 1)
                {
                    buf[n] = 0;
                    break;
                }
            }
        }
        exec(buf);
    }
}