.global irq_stub_table
irq_stub_table:
    .long irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15

.macro ISR n
.global isr\n
isr\n:
    cli
    pushl $0
    pushl $\n
    pushal
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    pushl %esp
    call isr_handler
    add $4, %esp
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popal
    add $8, %esp
    sti
    iret
.endm

ISR 0; ISR 1; ISR 2; ISR 3; ISR 4; ISR 5; ISR 6; ISR 7; ISR 8; ISR 9; ISR 10; ISR 11; ISR 12; ISR 13; ISR 14; ISR 15; ISR 16; ISR 17; ISR 18; ISR 19; ISR 20; ISR 21; ISR 22; ISR 23; ISR 24; ISR 25; ISR 26; ISR 27; ISR 28; ISR 29; ISR 30; ISR 31

.macro IRQ n
.global irq\n
irq\n:
    cli
    pushl $0
    pushl $\n
    pushal
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    pushl %esp
    call irq_handler
    add $4, %esp
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popal
    add $8, %esp
    sti
    iret
.endm

IRQ 0; IRQ 1; IRQ 2; IRQ 3; IRQ 4; IRQ 5; IRQ 6; IRQ 7; IRQ 8; IRQ 9; IRQ 10; IRQ 11; IRQ 12; IRQ 13; IRQ 14; IRQ 15