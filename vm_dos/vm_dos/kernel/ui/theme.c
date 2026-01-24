// kernel/ui/theme.c
#include "theme.h"
static Theme CURRENT;

Theme THEME_DEFAULT = {.fg = 14, .bg = 0x00, .accent = 14, .dim = 0x08, .ok = 0x0A, .warn = 0x0E, .err = 0x0C};
Theme THEME_NIGHT = {.fg = 0x0F, .bg = 0x00, .accent = 0x01, .dim = 0x08, .ok = 0x0A, .warn = 0x06, .err = 0x04};
Theme THEME_OCEAN = {.fg = 0x0F, .bg = 0x03, .accent = 0x0B, .dim = 0x08, .ok = 0x0A, .warn = 0x0E, .err = 0x0C};

void theme_set(const Theme *t) { CURRENT = *t; }
const Theme *theme_get(void) { return &CURRENT; }
