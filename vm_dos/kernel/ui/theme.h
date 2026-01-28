// kernel/ui/theme.h
#pragma once
#include <stdint.h>

typedef struct
{
    uint8_t fg, bg, accent, dim, ok, warn, err;
} Theme;

extern const Theme THEME_DEFAULT;

static inline const Theme *theme_get(void) { return &THEME_DEFAULT; }
