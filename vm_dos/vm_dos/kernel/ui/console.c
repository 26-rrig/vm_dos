#include "console.h"

static int cx = 0, cy = 0;
static uint8_t CUR_FG = 0x0F;
static uint8_t CUR_BG = 0x00;

void con_putc_at(int x, int y, char c, uint8_t fg, uint8_t bg)
{
    if (x < 0 || y < 0 || x >= VGA_WIDTH || y >= VGA_HEIGHT)
        return;
    VGA_MEM[y * VGA_WIDTH + x] = vga_entry(c, fg, bg);
}

void con_set_color(uint8_t fg, uint8_t bg)
{
    CUR_FG = fg;
    CUR_BG = bg;
}

void con_clear(uint8_t bg)
{
    CUR_BG = bg;
    for (int y = 0; y < VGA_HEIGHT; y++)
        for (int x = 0; x < VGA_WIDTH; x++)
            VGA_MEM[y * VGA_WIDTH + x] = vga_entry(' ', CUR_FG, bg);
    cx = 0;
    cy = 0;
}

void con_write(const char *s)
{
    while (*s)
    {
        if (*s == '\n')
        {
            cx = 0;
            if (++cy >= VGA_HEIGHT)
                cy = VGA_HEIGHT - 1;
        }
        else
        {
            con_putc_at(cx, cy, *s, CUR_FG, CUR_BG);
            if (++cx >= VGA_WIDTH)
            {
                cx = 0;
                if (++cy >= VGA_HEIGHT)
                    cy = VGA_HEIGHT - 1;
            }
        }
        s++;
    }
}

void con_write_at(int col, int row, const char *s)
{
    int x = col, y = row;
    while (*s && y >= 0 && y < VGA_HEIGHT)
    {
        if (*s == '\n')
        {
            x = col;
            y++;
        }
        else
        {
            if (x >= 0 && x < VGA_WIDTH)
                con_putc_at(x, y, *s, CUR_FG, CUR_BG);
            x++;
        }
        s++;
    }
}

void con_right_at(int row, const char *s)
{
    int len = 0;
    for (const char *p = s; *p; ++p)
        len++;
    int x = VGA_WIDTH - (len + 1);
    if (x < 0)
        x = 0;
    con_write_at(x, row, s);
}

void con_box(int x1, int y1, int x2, int y2, uint8_t fg, uint8_t bg, const char *title)
{
    if (x1 < 0)
        x1 = 0;
    if (y1 < 0)
        y1 = 0;
    if (x2 >= VGA_WIDTH)
        x2 = VGA_WIDTH - 1;
    if (y2 >= VGA_HEIGHT)
        y2 = VGA_HEIGHT - 1;
    if (x2 < x1 || y2 < y1)
        return;

    for (int y = y1; y <= y2; y++)
        for (int x = x1; x <= x2; x++)
            con_putc_at(x, y, ' ', fg, bg);

    char h = '=', v = '|', tl = '[', tr = ']', bl = '[', br = ']';
    for (int x = x1; x <= x2; x++)
    {
        con_putc_at(x, y1, h, fg, bg);
        con_putc_at(x, y2, h, fg, bg);
    }
    for (int y = y1; y <= y2; y++)
    {
        con_putc_at(x1, y, v, fg, bg);
        con_putc_at(x2, y, v, fg, bg);
    }
    con_putc_at(x1, y1, tl, fg, bg);
    con_putc_at(x2, y1, tr, fg, bg);
    con_putc_at(x1, y2, bl, fg, bg);
    con_putc_at(x2, y2, br, fg, bg);

    if (title && *title)
    {
        int len = 0;
        for (const char *p = title; *p; ++p)
            len++;
        int start = x1 + ((x2 - x1 + 1) - len) / 2;
        if (start < x1 + 1)
            start = x1 + 1;
        for (int i = 0; i < len && (start + i) < x2; i++)
            con_putc_at(start + i, y1, title[i], fg, bg);
    }
}

static void write_char(char c)
{
    char buf[2] = {c, 0};
    con_write(buf);
}

void con_write_hex(uint32_t v)
{
    con_write("0x");
    for (int i = 7; i >= 0; --i)
    {
        uint8_t n = (v >> (i * 4)) & 0xF;
        write_char(n < 10 ? ('0' + n) : ('A' + (n - 10)));
    }
}

void con_write_dec(uint32_t v)
{
    if (v == 0)
    {
        con_write("0");
        return;
    }
    char buf[10];
    int i = 0;
    while (v && i < 10)
    {
        buf[i++] = '0' + (v % 10);
        v /= 10;
    }
    while (i--)
        write_char(buf[i]);
}

void console_write(const char *s) { con_write(s); }
void console_write_hex(uint32_t v) { con_write_hex(v); }
void console_write_dec(uint32_t v) { con_write_dec(v); }
