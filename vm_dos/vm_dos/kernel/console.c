#include "console.h"
#include <stdint.h>

#define VGA_MEM ((volatile uint16_t *)0xB8000)
#define VGA_W 80
#define VGA_H 25

static uint8_t cur_x = 0, cur_y = 0;
static uint8_t color = 0x07; // fg=light gray, bg=black

// Correct inline assembly helpers
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "d"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "d"(port));
    return ret;
}

static void update_cursor(void)
{
    uint16_t pos = cur_y * VGA_W + cur_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void console_init(void)
{
    console_clear();
    update_cursor();
}
void console_set_color(uint8_t fg, uint8_t bg) { color = (bg << 4) | (fg & 0x0F); }

void console_clear(void)
{
    for (int y = 0; y < VGA_H; y++)
    {
        for (int x = 0; x < VGA_W; x++)
        {
            VGA_MEM[y * VGA_W + x] = ((uint16_t)color << 8) | ' ';
        }
    }
    cur_x = 0;
    cur_y = 0;
}

static void newline(void)
{
    cur_x = 0;
    if (++cur_y >= VGA_H)
    {
        cur_y = VGA_H - 1;
    }
}

void console_putc(char c)
{
    if (c == '\n')
    {
        newline();
        update_cursor();
        return;
    }
    VGA_MEM[cur_y * VGA_W + cur_x] = ((uint16_t)color << 8) | (uint8_t)c;
    if (++cur_x >= VGA_W)
    {
        newline();
    }
    update_cursor();
}

void console_write(const char *s)
{
    while (*s)
        console_putc(*s++);
}

void console_write_hex(uint32_t v)
{
    static const char *hex = "0123456789ABCDEF";
    console_write("0x");
    for (int i = 7; i >= 0; i--)
    {
        console_putc(hex[(v >> (i * 4)) & 0xF]);
    }
}

void console_write_dec(uint32_t v)
{
    char buf[11];
    int i = 10;
    buf[i] = 0;
    if (v == 0)
    {
        console_putc('0');
        return;
    }
    while (v && i)
    {
        buf[--i] = '0' + (v % 10);
        v /= 10;
    }
    console_write(buf + i);
}
