.global idt_flush
idt_flush:
    movl 4(%esp), %eax   /* arg: pointer to idt_ptr */
    lidt (%eax)
    ret
