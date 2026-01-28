#include <stdint.h>
#include "pic.h"

static inline void outb(uint16_t p, uint8_t v)
{
    __asm__ __volatile__("outb %0, %1" : : "a"(v), "Nd"(p));
}

void pic_remap(void)
{
    // PIC ports
    uint8_t ICW1_INIT = 0x10, ICW1_ICW4 = 0x01;
    uint8_t PIC1 = 0x20, PIC2 = 0xA0, PIC1_DATA = 0x21, PIC2_DATA = 0xA1;
    outb(PIC1, ICW1_INIT | ICW1_ICW4);
    outb(PIC2, ICW1_INIT | ICW1_ICW4);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);
}
