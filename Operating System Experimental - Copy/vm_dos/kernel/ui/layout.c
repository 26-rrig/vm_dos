// kernel/ui/layout.c
#include "ui/console.h"
#include "theme.h"

void ui_draw_chrome(void)
{
    const Theme *t = theme_get();
    con_set_color(t->fg, t->bg);
    con_clear(t->bg);

    // Top bar (accent)
    for (int x = 0; x < VGA_WIDTH; x++)
        con_putc_at(x, 0, ' ', t->fg, t->accent);
    con_write_at(2, 0, "vm_dos v0.0.1p0");

    // Left menu
    con_set_color(t->fg, t->bg);
    con_box(0, 1, 22, 22, t->fg, t->bg, " Main Menu ");
    con_write_at(2, 3, "• Files");
    con_write_at(2, 4, "• System");

    // Main shell panel
    con_box(23, 1, 57, 22, t->fg, t->bg, " vm_dos Shell ");

    // Status bar
    for (int x = 0; x < VGA_WIDTH; x++)
        con_putc_at(x, 24, ' ', t->dim, t->accent);
    con_write_at(2, 24, "• ESC = menu\n• F1 = help");
}
