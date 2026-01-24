// kernel/ui/theme.c
#include "theme.h"

Theme THEME_DEFAULT = {.fg = 0x0F, .bg = 0x01, .accent = 0x09, .dim = 0x08, .ok = 0x0A, .warn = 0x0E, .err = 0x0C};

const Theme *theme_get(void) { return &THEME_DEFAULT; }
