#include <stdint.h>
#include "gdt.h"

struct __attribute__((packed)) gdt_entry
{
    uint16_t limit_low, base_low;
    uint8_t base_mid, access, gran, base_high;
};
struct __attribute__((packed)) gdt_ptr
{
    uint16_t limit;
    uint32_t base;
};

static struct gdt_entry gdt[3];
static struct gdt_ptr gp;

static void gdt_set(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[i].limit_low = limit & 0xFFFF;
    gdt[i].base_low = base & 0xFFFF;
    gdt[i].base_mid = (base >> 16) & 0xFF;
    gdt[i].access = access;
    gdt[i].gran = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[i].base_high = (base >> 24) & 0xFF;
}

void gdt_init(void)
{
    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)&gdt;

    // null, kernel code, kernel data
    gdt_set(0, 0, 0, 0, 0);
    gdt_set(1, 0, 0xFFFFF, 0x9A, 0xCF);
    gdt_set(2, 0, 0xFFFFF, 0x92, 0xCF);

    // Load GDT
    __asm__ __volatile__("lgdt (%0)" : : "r"(&gp));

    // Reload segment registers (AT&T syntax, single % for registers)
    __asm__ __volatile__(
        "movw $0x10, %ax\n"
        "movw %ax, %ds\n"
        "movw %ax, %es\n"
        "movw %ax, %fs\n"
        "movw %ax, %gs\n"
        "movw %ax, %ss\n"
        // Far jump to flush prefetch queue and load CS with 0x08
        "ljmp $0x08, $1f\n"
        "1:\n");
}
