// kernel/arch/x86/kbd.h
#pragma once

int kbd_getchar_blocking(void);
void kbd_readline(char *buf, int max);
void kbd_init(void);

#ifndef KBD_H
#define KBD_H

#include <stdint.h>

// Special key codes
#define KEY_ESC 0x1B
#define KEY_BACKSPACE 0x08

// Function keys mapped to custom codes beyond ASCII
#define KEY_F1 0x80
#define KEY_F2 0x81
#define KEY_F3 0x82
#define KEY_F4 0x83
#define KEY_F5 0x84
#define KEY_F6 0x85
#define KEY_F7 0x86
#define KEY_F8 0x87
#define KEY_F9 0x88
#define KEY_F10 0x89
#define KEY_F11 0x8A
#define KEY_F12 0x8B

// Translate raw scancode to keycode
char kbd_translate(uint8_t scancode);

#endif
