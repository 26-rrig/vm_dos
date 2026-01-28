// kernel/ui/layout.c
#include "ui/console.h"
#include "theme.h"
#include "ui/layout.h"
#include "docs/VERSION.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static int fullscreen_mode = 0;

// Write keyboard text to the bottom yellow status bar

void ui_write_keyboard_input(const char *text)
{
    const Theme *t = theme_get();

    // Clear the right side of the status bar
    for (int x = 30; x < VGA_WIDTH; x++)
        con_putc_at(x, 24, ' ', t->dim, t->accent);

    // Write new text
    con_write_at(30, 24, text);
}

// Fullscreen shell mode
void ui_draw_fullscreen_shell(void)
{
    const Theme *t = theme_get();
    con_set_color(t->fg, t->bg);
    con_clear(t->bg);

    // Top bar
    for (int x = 0; x < VGA_WIDTH; x++)
        con_putc_at(x, 0, ' ', t->fg, t->accent);
    con_write_at(56, 0, " vm_dos v1.0.1 - alpha ");

    // Status bar at bottom
    for (int x = 0; x < VGA_WIDTH; x++)
        con_putc_at(x, 24, ' ', t->dim, t->accent);
    con_write_at(2, 24, " • Enter: fullscreen shell. • help: commands. ");

    fullscreen_mode = 1;
}

// Draw file browser in left panel
void ui_draw_file_browser(void)
{
    const Theme *t = theme_get();
    con_box(0, 1, 22, 22, t->fg, t->bg, " FILES ");
    con_set_color(t->fg, t->bg);
    con_write_at(2, 2, "Root directory");
    con_write_at(2, 4, "~/ /root");
}

// Update info panel with system info
void ui_update_info_panel(void)
{
    const Theme *t = theme_get();
    con_set_color(t->fg, t->bg);

    // Clear the info panel
    for (int y = 2; y < 22; y++)
        for (int x = 60; x < 78; x++)
            con_putc_at(x, y, ' ', t->fg, t->bg);

    // Add system info
    con_write_at(60, 2, __EDG_VERSION__);
    con_write_at(60, 4, "Status: Running");
    con_write_at(60, 6, "Shell: Active");
    con_write_at(60, 8, "Mem: OK");
}

// Draw the full UI chrome

void ui_draw_chrome(void)
{
    const Theme *t = theme_get();
    con_set_color(t->fg, t->bg);
    con_clear(t->bg);

    // Top bar (accent)
    for (int x = 0; x < VGA_WIDTH; x++)
        con_putc_at(x, 0, ' ', t->fg, t->accent);
    con_write_at(56, 0, " vm_dos v1.0.1 - alpha ");

    // Left panel - File browser
    ui_draw_file_browser();

    // Main shell panel
    con_box(23, 1, 57, 22, t->fg, t->bg, " SYSTEM: vm_dos Shell ");

    // Right info panel
    con_box(58, 1, 79, 22, t->fg, t->bg, " SYSTEM: Info ");
    ui_update_info_panel();

    // Status bar
    for (int x = 0; x < VGA_WIDTH; x++)
        con_putc_at(x, 24, ' ', t->dim, t->accent);
    con_write_at(2, 24, " • Enter: fullscreen shell. • help: commands. ");

    fullscreen_mode = 0;
}

int ui_is_fullscreen(void)
{
    return fullscreen_mode;
}
