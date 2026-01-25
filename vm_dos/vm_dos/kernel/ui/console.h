#pragma once
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEM ((volatile uint16_t *)0xB8000)

static inline uint16_t vga_entry(char c, uint8_t fg, uint8_t bg)
{
    uint8_t attr = ((bg & 0x0F) << 4) | (fg & 0x0F);
    return ((uint16_t)attr << 8) | (uint16_t)c;
}

void con_clear(uint8_t bg);
void con_set_color(uint8_t fg, uint8_t bg);
void con_write(const char *s);
void con_write_at(int row, int col, const char *s);
void con_right_at(int row, const char *s);
void con_putc_at(int x, int y, char c, uint8_t fg, uint8_t bg);
void con_box(int x1, int y1, int x2, int y2, uint8_t fg, uint8_t bg, const char *title);
void con_write_hex(uint32_t v);
void con_write_dec(uint32_t v);

void console_write(const char *s);
void console_write_hex(uint32_t v);
void console_write_dec(uint32_t v);
