// kernel/ui/theme.c
#include "theme.h"

const Theme THEME_DEFAULT = {
    .fg = 0x0F,     // foreground: bright white-blue (mapped from #ECEFF4)
    .bg = 0x00,     // background: deep slate (mapped from #2E3440)
    .accent = 0x0B, // accent: cool cyan (mapped from #88C0D0)
    .dim = 0x08,    // dim: steel grey (mapped from #4C566A)
    .ok = 0x0A,     // success: soft green (mapped from #A3BE8C)
    .warn = 0x0E,   // warning: warm amber (mapped from #EBCB8B)
    .err = 0x0C     // error: muted red (mapped from #BF616A)
};
