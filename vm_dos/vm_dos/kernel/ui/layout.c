// kernel/ui/layout.c
#include "ui/console.h"
#include "theme.h"

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

// Draw the full UI chrome

void ui_draw_chrome(void)
{
    const Theme *t = theme_get();
    con_set_color(t->fg, t->bg);
    con_clear(t->bg);

    // Top bar (accent)
    for (int x = 0; x < VGA_WIDTH; x++)
        con_putc_at(x, 0, ' ', t->fg, t->accent);
    con_write_at(0, 57, "vm_dos v0.0.1p1 Alpha.");

    // Left menu
    con_set_color(t->fg, t->bg);
    con_box(0, 1, 22, 22, t->fg, t->bg, " SYSTEM: Main Menu ");

    // Main shell panel
    con_box(23, 1, 57, 22, t->fg, t->bg, " SYSTEM: vm_dos Shell ");

    // Right info panel (fits inside 80 columns)
    con_box(58, 1, 79, 22, t->fg, t->bg, " SYSTEM: Info Panel ");

    // Status bar
    for (int x = 0; x < VGA_WIDTH; x++)
        con_putc_at(x, 24, ' ', t->dim, t->accent);
    con_write_at(2, 24, "- ESC = menu | - F1 = help");
}
