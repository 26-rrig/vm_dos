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
extern volatile uint32_t pit_ticks;

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

static int output_line = 8;

void shell_print(const char *text)
{
    const Theme *t = theme_get();

    // In fullscreen mode, print to full width starting at current output_line
    if (ui_is_fullscreen())
    {
        if (output_line >= 23)
        {
            // Scroll content up
            for (int y = 2; y < 23; y++)
            {
                for (int x = 1; x < 79; x++)
                {
                    uint16_t c = VGA_MEM[y * 80 + x];
                    VGA_MEM[(y - 1) * 80 + x] = c;
                }
            }
            // Clear bottom line
            for (int x = 1; x < 79; x++)
                con_putc_at(x, 22, ' ', t->fg, t->bg);
            output_line = 22;
        }
        con_write_at(1, output_line, text);
        output_line++;
    }
    else
    {
        // In normal mode, print within shell box
        if (output_line >= 21)
        {
            // Scroll
            for (int y = 4; y < 21; y++)
            {
                for (int x = 24; x < 57; x++)
                {
                    uint16_t c = VGA_MEM[y * 80 + x];
                    VGA_MEM[(y - 1) * 80 + x] = c;
                }
            }
            for (int x = 24; x < 57; x++)
                con_putc_at(x, 20, ' ', t->fg, t->bg);
            output_line = 20;
        }
        con_write_at(25, output_line, text);
        output_line++;
    }
}

void shell_run(void)
{
    const Theme *t = theme_get();
    con_set_color(t->fg, t->bg);

    char line[256];
    int fullscreen = 0;

    shell_print("Press ENTER for fullscreen");
    shell_print("Type 'help' to begin.");

    while (1)
    {
        // Clear and draw prompt line
        for (int i = 0; i < 30; i++)
        {
            if (fullscreen)
                con_putc_at(1 + i, 23, ' ', t->fg, t->bg);
            else
                con_putc_at(25 + i, 5, ' ', t->fg, t->bg);
        }

        int prompt_row = fullscreen ? 23 : 5;
        int prompt_col = fullscreen ? 1 : 25;
        con_write_at(prompt_col, prompt_row, "$ > ");

        kbd_readline(line, sizeof(line));

        // Check for fullscreen toggle (empty input)
        if (strlen(line) == 0)
        {
            fullscreen = !fullscreen;
            if (fullscreen)
            {
                ui_draw_fullscreen_shell();
                output_line = 2;
                shell_print(" Entered Fullscreen Shell Mode.");
            }
            else
            {
                ui_draw_chrome();
                output_line = 8;
                shell_print("Returned to normal mode");
            }
            continue;
        }

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
            char errmsg[50];
            int pos = 0;
            const char *prefix = "ERR: Unknown: ";
            for (const char *p = prefix; *p && pos < 49; p++)
                errmsg[pos++] = *p;
            for (const char *p = argv[0]; *p && pos < 49; p++)
                errmsg[pos++] = *p;
            errmsg[pos] = 0;
            shell_print(errmsg);
            con_set_color(t->fg, t->bg);
        }
    }
}
