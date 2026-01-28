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
    extern void shell_print(const char *text);
    shell_print("Available commands:");
    shell_print("  help    - Show this message");
    shell_print("  clear   - Clear screen");
    shell_print("  echo    - Print text");
    shell_print("  ls      - List files");
    shell_print("  cat     - Show file contents");
    shell_print("  toast   - Show notification");
    shell_print("  status  - Show system status");
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
    extern void shell_print(const char *text);
    char msg[80];
    int pos = 0;
    for (int i = 1; i < a && pos < 78; i++)
    {
        for (const char *p = v[i]; *p && pos < 78; p++)
            msg[pos++] = *p;
        if (i < a - 1 && pos < 78)
            msg[pos++] = ' ';
    }
    msg[pos] = 0;
    shell_print(msg);
    return 0;
}
static int cmd_panel(int a, const char **v)
{
    extern void shell_print(const char *text);
    shell_print("System panel refreshed.");
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
    extern void shell_print(const char *text);
    if (a < 2)
    {
        shell_print("Usage: cat <path>");
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
static int cmd_status(int a, const char **v)
{
    extern void shell_print(const char *text);
    extern volatile uint32_t pit_ticks;

    shell_print("-- System Status --");
    shell_print("Status: Running OK");

    int seconds = pit_ticks / 100;
    int mins = seconds / 60;
    int secs = seconds % 60;

    char buf[40];
    int pos = 0;
    const char *prefix = "Uptime: ";
    for (const char *p = prefix; *p && pos < 39; p++)
        buf[pos++] = *p;

    if (mins > 0)
    {
        // Simple itoa for mins
        int temp = mins;
        int digits = 0;
        int temp2 = temp;
        if (temp2 == 0)
            digits = 1;
        while (temp2 > 0)
        {
            digits++;
            temp2 /= 10;
        }

        for (int i = digits - 1; i >= 0 && pos < 39; i--)
        {
            buf[pos++] = '0' + ((temp / (1)) % 10);
            temp /= 10;
        }
        if (pos < 39)
            buf[pos++] = 'm';
        if (pos < 39)
            buf[pos++] = ' ';
    }

    if (secs > 0 || mins == 0)
    {
        int temp = secs;
        int digits = 0;
        int temp2 = temp;
        if (temp2 == 0)
            digits = 1;
        while (temp2 > 0)
        {
            digits++;
            temp2 /= 10;
        }

        for (int i = digits - 1; i >= 0 && pos < 39; i--)
        {
            buf[pos++] = '0' + ((temp / (1)) % 10);
            temp /= 10;
        }
        if (pos < 39)
            buf[pos++] = 's';
    }

    buf[pos] = 0;
    shell_print(buf);

    shell_print("Kernel: v1.0.1");
    shell_print("Shell: Active");
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
    {"status", "Show system status", cmd_status},
    {0, 0, 0}};
