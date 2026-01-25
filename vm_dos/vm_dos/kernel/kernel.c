// kernel/kernel.c
#include "arch/x86/gdt.h"
#include "arch/x86/idt.h"
#include "arch/x86/isr.h"
#include "arch/x86/pic.h"
#include "arch/x86/pit.h"
#include "arch/x86/kbd.h"
#include "arch/x86/pmm.h"
#include "arch/x86/paging.h"
#include "fs/vfs.h"
#include "fs/initrd.h"
#include "syscalls.h"
#include "user/run_init.h"

#include "ui/theme.h"
#include "ui/console.h"
#include "ui/layout.h"
#include "ui/anim.h"
#include "ui/toast.h"
#include "shell/shell.h"

extern volatile uint32_t pit_ticks;
extern void run_init(void); // declare userland init

void kmain(void)
{
    con_set_color(0x0A, 0x00);
    con_write("\n[vm_dos v0.0.2p1 Alpha] UI init...\n");

    // Core system setup...
    gdt_init();
    idt_init();
    isr_init();
    pic_remap();
    pmm_init();
    paging_init();
    pit_init(100);
    kbd_init();

    vfs_init();
    initrd_mount();

    syscalls_init();
    run_init();

    __asm__ volatile("sti");

    con_set_color(theme_get()->fg, theme_get()->bg);
    con_clear(theme_get()->bg);
    ui_draw_chrome();

    anim_init();
    toast_show("vm_dos: System Boot Successful.", 300);

    shell_run();
}
