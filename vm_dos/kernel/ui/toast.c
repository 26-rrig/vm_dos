// kernel/ui/toast.c
#include "toast.h"
#include "console.h"
#include "theme.h"

typedef struct
{
    const char *msg;
    uint32_t until;
} Toast;
static Toast current = {0};

void toast_show(const char *msg, uint32_t duration_ticks)
{
    extern uint32_t pit_ticks;
    current.msg = msg;
    current.until = pit_ticks + duration_ticks;
    const Theme *t = theme_get();
    con_set_color(t->ok, t->bg);
    con_right_at(23, msg);
    con_set_color(t->fg, t->bg);
}

void toast_tick(uint32_t tick)
{
    if (current.msg && tick >= current.until)
    {
        current.msg = 0;
        const Theme *t = theme_get();
        con_set_color(t->dim, t->bg);
        con_right_at(23, "                ");
        con_set_color(t->fg, t->bg);
    }
}
