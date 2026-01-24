// kernel/arch/x86/kbd.h
#pragma once

int kbd_getchar_blocking(void);
void kbd_readline(char *buf, int max);
void kbd_init(void);
