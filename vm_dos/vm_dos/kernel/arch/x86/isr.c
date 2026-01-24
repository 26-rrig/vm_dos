#include <stdint.h>
#include "isr.h"
#include "idt.h"
#include "console.h"

// Basic CPU register frame passed from ISR stubs
typedef struct regs
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} regs_t;

typedef void (*isr_t)(regs_t *);

static isr_t interrupt_handlers[256] = {0};

// Optional: minimal assertion/print helper
static void print_hex32(const char *label, uint32_t v)
{
    console_write(label);
    console_write_hex(v);
    console_write("\n");
}

// Load kernel data segments (0x10) — fixes your previous %%ax errors
static inline void load_kernel_segments(void)
{
    __asm__ __volatile__(
        "movw $0x10, %ax\n"
        "movw %ax, %ds\n"
        "movw %ax, %es\n"
        "movw %ax, %fs\n"
        "movw %ax, %gs\n"
        "movw %ax, %ss\n");
}

// Common C-side ISR handler
void isr_common_handler(regs_t *r)
{
    // Ensure we’re on kernel segments (important after privilege changes)
    load_kernel_segments();

    if (interrupt_handlers[r->int_no])
    {
        interrupt_handlers[r->int_no](r);
    }
    else
    {
        console_write("Unhandled interrupt: ");
        console_write_dec(r->int_no);
        console_write("\n");
        print_hex32("EIP: ", r->eip);
        print_hex32("CS:  ", r->cs);
        print_hex32("EFLAGS: ", r->eflags);
    }
}

// Register a handler
void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

// ISR install — set IDT gates for CPU exceptions (0–31)
// Your project likely already has idt_set_gate() and idt_install() elsewhere.
// Keep or adapt these externs to match your stubs (provided by boot.S or idt.S).
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void isr_install(void)
{
    // Gate type: 0x8E = present, DPL=0, 32-bit interrupt gate
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
}

// Example default handlers for CPU exceptions
static const char *exc_names[32] = {
    "Divide-by-zero", "Debug", "NMI", "Breakpoint", "Overflow", "BOUND Range",
    "Invalid opcode", "Device not available", "Double fault", "Coprocessor segment overrun",
    "Invalid TSS", "Segment not present", "Stack fault", "General protection", "Page fault", "Reserved",
    "x87 FPU", "Alignment check", "Machine check", "SIMD Floating-Point", "Virtualization", "Control Protection",
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved"};

void isr_default(regs_t *r)
{
    console_write("Exception: ");
    if (r->int_no < 32)
        console_write(exc_names[r->int_no]);
    else
        console_write("Unknown");
    console_write("\n");
    print_hex32("Error code: ", r->err_code);
    print_hex32("EIP: ", r->eip);
    print_hex32("CS:  ", r->cs);
    print_hex32("EFLAGS: ", r->eflags);
}

// Allow users to set defaults
void isr_set_defaults(void)
{
    for (int i = 0; i < 32; i++)
        register_interrupt_handler(i, isr_default);
}

// Wrapper so kernel can call isr_init()
void isr_init(void)
{
    isr_install();
}

// If idt.c expects isr_stub_addrs[], export it here
void *isr_stub_addrs[32] = {
    isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7,
    isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15,
    isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
    isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31};

#include "idt.h"

/*
Notes:
- This file assumes your ISR stubs (isr0..isr31) are defined in assembly (e.g., isr.S),
  and that each stub ultimately calls isr_common_handler(regs_t*).
- If your current isr.c contains inline assembly stubs with "%%ax", replace those blocks
  with load_kernel_segments() or convert them to AT&T syntax as shown:
    "movw $0x10, %ax\n"
    "movw %ax, %ds\n" ... etc.
- If your project uses a different regs_t layout, adjust the struct to match your push order.
*/
