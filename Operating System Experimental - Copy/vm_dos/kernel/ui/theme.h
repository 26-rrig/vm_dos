// kernel/ui/theme.h
#pragma once
#include <stdint.h>

typedef struct
{
    uint8_t fg, bg, accent, dim, ok, warn, err;
} Theme;

extern Theme THEME_DEFAULT;

const Theme *theme_get(void);
