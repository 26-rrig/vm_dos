// kernel/ui/theme.h
#pragma once
#include <stdint.h>

typedef struct
{
    uint8_t fg, bg, accent, dim, ok, warn, err;
} Theme;

extern Theme THEME_DEFAULT;
extern Theme THEME_NIGHT;
extern Theme THEME_OCEAN;

void theme_set(const Theme *t);
const Theme *theme_get(void);
