// kernel/ui/theme.c
#include "theme.h"
static Theme CURRENT;

Theme THEME_DEFAULT = {
    .fg = 0x0F,     // foreground: bright white-blue (mapped from #ECEFF4)
    .bg = 0x00,     // background: deep slate (mapped from #2E3440)
    .accent = 0x0B, // accent: cool cyan (mapped from #88C0D0)
    .dim = 0x08,    // dim: steel grey (mapped from #4C566A)
    .ok = 0x0A,     // success: soft green (mapped from #A3BE8C)
    .warn = 0x0E,   // warning: warm amber (mapped from #EBCB8B)
    .err = 0x0C     // error: muted red (mapped from #BF616A)
};

// Theme THEME_DEFAULT = {.fg = 14, .bg = 0x03, .accent = 14, .dim = 0x08, .ok = 0x0A, .warn = 0x0E, .err = 0x0C};
Theme THEME_NIGHT = {.fg = 0x0F, .bg = 0x00, .accent = 0x01, .dim = 0x08, .ok = 0x0A, .warn = 0x06, .err = 0x04};
Theme THEME_OCEAN = {.fg = 0x0F, .bg = 0x03, .accent = 0x0B, .dim = 0x08, .ok = 0x0A, .warn = 0x0E, .err = 12}; // 0x0C Previous
void theme_set(const Theme *t)
{
    CURRENT = *t;
}
const Theme *theme_get(void) { return &CURRENT; }
