// kernel/arch/x86/kbd_readline.c (or inside kbd.c)
#include <stdint.h>
#include "ui/console.h"
#include "ui/theme.h"

extern int kbd_getchar_blocking(void); // return ASCII, handle scancodes elsewhere

void kbd_readline(char *buf, int max)
{
    int len = 0;
    const Theme *t = theme_get();
    con_set_color(t->fg, t->bg);
    while (1)
    {
        int c = kbd_getchar_blocking();
        if (c == '\r' || c == '\n')
        {
            buf[len] = 0;
            con_write("\n");
            return;
        }
        if (c == 8)
        { // backspace
            if (len > 0)
            {
                len--;
                con_write("\b \b");
            }
            continue;
        }
        if (len < max - 1 && c >= 32 && c <= 126)
        {
            buf[len++] = (char)c;
            char s[2] = {(char)c, 0};
            con_write(s);
        }
    }
}
