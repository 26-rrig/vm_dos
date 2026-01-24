// kernel/arch/x86/kbd.c
#include "arch/x86/kbd.h"
#include "ui/console.h"
#include <stdint.h>
#include "arch/x86/io.h"

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
