// kernel/arch/x86/kbd.c
#include "arch/x86/kbd.h"
#include "ui/console.h"
#include <stdint.h>
#include "arch/x86/io.h"
#include "kbd.h"

// Basic US keyboard scancode → ASCII map (set 1)
static const char ascii_map[128] = {
    0,
    27,
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
    '=',
    '\b', // 0x0E backspace
    '\t', // 0x0F tab
    '\r', // 0x10 enter
    0,    // 0x11 ctrl
    'q',
    'w',
    'e',
    'r',
    't',
    'y',
    'u',
    'i',
    'o',
    'p',
    '[',
    ']',
    '\n',
    0, // 0x1C enter
    'a',
    's',
    'd',
    'f',
    'g',
    'h',
    'j',
    'k',
    'l',
    ';',
    '\'',
    '`',
    0,
    '\\',
    'z',
    'x',
    'c',
    'v',
    'b',
    'n',
    'm',
    ',',
    '.',
    '/',
    0,
    '*',
    0,
    ' ', // 0x39 space
    // fill rest with 0
};

char kbd_translate(uint8_t scancode)
{
    switch (scancode)
    {
    case 0x01:
        return KEY_ESC; // ESC
    case 0x0E:
        return KEY_BACKSPACE; // Backspace
    case 0x3B:
        return KEY_F1;
    case 0x3C:
        return KEY_F2;
    case 0x3D:
        return KEY_F3;
    case 0x3E:
        return KEY_F4;
    case 0x3F:
        return KEY_F5;
    case 0x40:
        return KEY_F6;
    case 0x41:
        return KEY_F7;
    case 0x42:
        return KEY_F8;
    case 0x43:
        return KEY_F9;
    case 0x44:
        return KEY_F10;
    case 0x57:
        return KEY_F11;
    case 0x58:
        return KEY_F12;
    default:
        // fall back to your existing ASCII map
        return ascii_map[scancode];
    }
}

// External I/O helpers (declare if not in a common header)
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t val);

// Minimal scancode -> ASCII mapping (Set 1, partial)
static int scancode_to_ascii(uint8_t sc)
{
    // Ignore releases
    if (sc & 0x80)
        return -1;

    // Enter, Backspace
    if (sc == 0x1C)
        return '\n';
    if (sc == 0x0E)
        return 8;

    // Number row (no shift)
    if (sc >= 0x02 && sc <= 0x0B)
    {
        static const char nums[] = "1234567890";
        return nums[sc - 0x02];
    }

    // Letters (partial)
    switch (sc)
    {
    case 0x10:
        return 'q';
    case 0x11:
        return 'w';
    case 0x12:
        return 'e';
    case 0x13:
        return 'r';
    case 0x14:
        return 't';
    case 0x15:
        return 'y';
    case 0x16:
        return 'u';
    case 0x17:
        return 'i';
    case 0x18:
        return 'o';
    case 0x19:
        return 'p';
    case 0x1E:
        return 'a';
    case 0x1F:
        return 's';
    case 0x20:
        return 'd';
    case 0x21:
        return 'f';
    case 0x22:
        return 'g';
    case 0x23:
        return 'h';
    case 0x24:
        return 'j';
    case 0x25:
        return 'k';
    case 0x26:
        return 'l';
    case 0x2C:
        return 'z';
    case 0x2D:
        return 'x';
    case 0x2E:
        return 'c';
    case 0x2F:
        return 'v';
    case 0x30:
        return 'b';
    case 0x31:
        return 'n';
    case 0x32:
        return 'm';
    }
    return -1;
}

// Poll the controller for a scancode and convert to ASCII
static int kbd_poll(void)
{
    // Status port 0x64 bit0 indicates output buffer full
    if ((inb(0x64) & 1) == 0)
        return -1;
    uint8_t sc = inb(0x60);
    return scancode_to_ascii(sc);
}

void kbd_init(void)
{
    con_write("[kbd] init\n");
    // If you wire IRQ1 elsewhere, enable it there. This stub leaves polling.
}

int kbd_getchar_blocking(void)
{
    int c;
    while ((c = kbd_poll()) == -1)
    { /* spin */
    }
    return c;
}

void kbd_readline(char *buf, int max)
{
    int len = 0;
    while (1)
    {
        int c = kbd_getchar_blocking();
        if (c == '\r' || c == '\n')
        {
            buf[len] = 0;
            con_write("\n");
            return;
        }
        if (c == 8)
        {
            if (len > 0)
            {
                len--;
                con_write("\b \b");
            }
            continue;
        }
        if (len < max - 1 && c >= 32 && c <= 126)
        {
            buf[len++] = (char)c;
            char s[2] = {(char)c, 0};
            con_write(s);
        }
    }
}
