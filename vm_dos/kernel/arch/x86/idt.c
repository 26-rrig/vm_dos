#include <stdint.h>
#include "idt.h"

struct __attribute__((packed)) idt_entry
{
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
};

struct __attribute__((packed)) idt_ptr
{
    uint16_t limit;
    uint32_t base;
};

static struct idt_entry idt[256];
static struct idt_ptr idtp;

extern void idt_flush(uint32_t); // lidt(&idtp)

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].base_low = base & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_high = (base >> 16) & 0xFFFF;
}

void idt_init(void)
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;

    // zero table
    for (int i = 0; i < 256; i++)
    {
        idt[i].base_low = 0;
        idt[i].sel = 0;
        idt[i].always0 = 0;
        idt[i].flags = 0;
        idt[i].base_high = 0;
    }

    idt_flush((uint32_t)&idtp);
}
