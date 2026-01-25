// kernel/shell/shell.c
#include "ui/console.h"
#include "ui/theme.h"
#include "ui/layout.h"
#include "ui/anim.h"
#include "ui/toast.h"
#include <string.h>
#include <stdint.h>

typedef int (*cmd_fn)(int argc, const char **argv);
typedef struct
{
    const char *name;
    const char *help;
    cmd_fn fn;
} Cmd;
extern Cmd COMMANDS[];

extern void kbd_readline(char *buf, int max); // implement in kbd.c

static void split(char *s, int *argc, const char **argv, int maxv)
{
    *argc = 0;
    char *p = s;
    while (*p && *argc < maxv)
    {
        while (*p == ' ' || *p == '\t')
            p++;
        if (!*p)
            break;
        argv[(*argc)++] = p;
        while (*p && *p != ' ' && *p != '\t')
            p++;
        if (*p)
        {
            *p = 0;
            p++;
        }
    }
}

void shell_run(void)
{
    const Theme *t = theme_get();
    con_set_color(t->fg, t->bg);
    con_write_at(25, 3, "Type 'help' to begin.");
    char line[256];
    while (1)
    {
        for (int i = 0; i < 30; i++)
            con_putc_at(25 + i, 5, ' ', t->fg, t->bg);
        con_write_at(25, 5, "> ");
        kbd_readline(line, sizeof(line));
        int argc;
        const char *argv[16];
        split(line, &argc, argv, 16);
        if (argc == 0)
            continue;
        int handled = 0;
        for (Cmd *c = COMMANDS; c->name; c++)
        {
            if (!strcmp(argv[0], c->name))
            {
                c->fn(argc, argv);
                handled = 1;
                break;
            }
        }
        if (!handled)
        {
            con_set_color(t->err, t->bg);
            con_write_at(25, 7, "Unknown command. Type 'help'.");
            con_set_color(t->fg, t->bg);
        }
    }
}
