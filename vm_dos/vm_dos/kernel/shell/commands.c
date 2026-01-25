// kernel/shell/commands.c
#include "ui/console.h"
#include "ui/theme.h"
#include "ui/layout.h"
#include "ui/toast.h"
#include "ui/anim.h"
#include <string.h>

// External VFS helpers
extern void vfs_list(const char *path);
extern void vfs_cat(const char *path);

// PIT sleep (non-busy preferred; stub ok)
extern void pit_sleep(int ticks);

typedef int (*cmd_fn)(int argc, const char **argv);
typedef struct
{
    const char *name;
    const char *help;
    cmd_fn fn;
} Cmd;

extern void ui_draw_chrome(void);

static int cmd_help(int argc, const char **argv)
{
    con_write_at(25, 8, "Commands: help, clear, ls, cat, echo, panel, progress, toast");
    return 0;
}
static int cmd_clear(int a, const char **v)
{
    const Theme *t = theme_get();
    con_clear(t->bg);
    ui_draw_chrome();
    return 0;
}
static int cmd_echo(int a, const char **v)
{
    int x = 25, y = 10;
    for (int i = 1; i < a; i++)
    {
        con_write_at(x, y, v[i]);
        x += (int)strlen(v[i]);
        con_write_at(x, y, " ");
        x++;
    }
    return 0;
}
static int cmd_panel(int a, const char **v)
{
    const Theme *t = theme_get();
    con_box(23, 1, 57, 22, t->fg, t->bg, " Shell ");
    con_write_at(25, 14, "Panel refreshed.");
    return 0;
}
static int cmd_progress(int a, const char **v)
{
    for (int i = 0; i <= 40; i++)
    {
        con_putc_at(25 + i, 24, '#', theme_get()->ok, theme_get()->bg);
        pit_sleep(3);
    }
    toast_show("Done", 200);
    return 0;
}
static int cmd_ls(int a, const char **v)
{
    vfs_list(a > 1 ? v[1] : "/");
    return 0;
}
static int cmd_cat(int a, const char **v)
{
    if (a < 2)
    {
        con_write_at(25, 18, "Usage: cat <path>");
        return 1;
    }
    vfs_cat(v[1]);
    return 0;
}
static int cmd_toast(int a, const char **v)
{
    if (a < 2)
    {
        toast_show("Toast!", 300);
        return 0;
    }
    toast_show(v[1], 300);
    return 0;
}

Cmd COMMANDS[] = {
    {"help", "Show commands", cmd_help},
    {"clear", "Clear screen", cmd_clear},
    {"echo", "Print text", cmd_echo},
    {"panel", "Redraw panel", cmd_panel},
    {"progress", "Animate progress bar", cmd_progress},
    {"ls", "List files", cmd_ls},
    {"cat", "Show file contents", cmd_cat},
    {"toast", "Show toast", cmd_toast},
    {0, 0, 0}};
