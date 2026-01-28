// kernel/arch/x86/pit.c
#include "arch/x86/pit.h"
#include <stdint.h>
#include "arch/x86/io.h"

// If you already have inb/outb helpers, include their header.
// Otherwise declare them here:
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t val);

// Global ticks (single definition)
volatile uint32_t pit_ticks = 0;

static void pit_set_rate(int hz)
{
    // PIT base is 1193180 Hz
    uint16_t divisor = (hz > 0) ? (1193180 / hz) : 1193180;
    outb(0x43, 0x36);                             // channel 0, lobyte/hibyte, mode 3, binary
    outb(0x40, (uint8_t)(divisor & 0xFF));        // low byte
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF)); // high byte
}

void pit_init(int hz)
{
    pit_set_rate(hz);
    // If you have an IRQ registration, hook IRQ0 to your handler here.
    // e.g., irq_install_handler(0, pit_irq_handler);
}

void pit_sleep(uint32_t ticks)
{
    uint32_t start = pit_ticks;
    while ((pit_ticks - start) < ticks)
    {
        // busy wait
    }
}

// This should be called by the IRQ0 handler
__attribute__((unused)) static void pit_irq_handler(/* regs if applicable */)
{
    pit_ticks++;
    // Acknowledge PIC if not handled elsewhere
    // outb(0x20, 0x20);  // end of interrupt for master PIC
}
