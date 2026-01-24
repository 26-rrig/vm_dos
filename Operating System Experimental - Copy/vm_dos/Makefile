.RECIPEPREFIX := >

TARGET=vm_dos
BUILD=build
ISO=iso

CC=gcc
AS=as --32
LD=ld -m elf_i386

CFLAGS=-m32 -ffreestanding -nostdlib -std=c11 -O2 -Wall -Wextra -Wno-unused-parameter -fno-builtin -I. -Ikernel -fno-pic -fno-pie

KERNEL_SRC= \
  kernel/lib/string.c \
  kernel/kernel.c \
  kernel/arch/x86/gdt.c \
  kernel/arch/x86/idt.c \
  kernel/arch/x86/isr.c \
  kernel/arch/x86/pic.c \
  kernel/arch/x86/pit.c \
  kernel/arch/x86/kbd.c \
  kernel/arch/x86/pmm.c \
  kernel/arch/x86/paging.c \
  kernel/syscalls.c \
  fs/vfs.c \
  fs/initrd.c \
  user/run_init.c \
  kernel/ui/console.c \
  kernel/ui/theme.c \
  kernel/ui/layout.c \
  kernel/ui/anim.c \
  kernel/ui/toast.c \
  kernel/ui/clock.c \
  kernel/shell/shell.c \
  kernel/shell/commands.c

KERNEL_ASM= \
  kernel/arch/x86/boot.S \
  kernel/arch/x86/isr_stubs.S \
  kernel/arch/x86/idt_flush.S

KERNEL_OBJ=$(KERNEL_SRC:%.c=$(BUILD)/%.o) $(KERNEL_ASM:%.S=$(BUILD)/%.o)

GRUB_CFG=boot/grub/grub.cfg

all: $(ISO)/$(TARGET).iso

$(BUILD)/%.o: %.c
> mkdir -p $(dir $@)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.S
> mkdir -p $(dir $@)
> $(AS) $< -o $@

$(BUILD)/$(TARGET).elf: $(KERNEL_OBJ) linker.ld
> $(LD) -T linker.ld -o $@ $(KERNEL_OBJ)

$(ISO)/$(TARGET).iso: $(BUILD)/$(TARGET).elf $(GRUB_CFG)
> rm -rf $(ISO)
> mkdir -p $(ISO)/boot/grub
> cp $(BUILD)/$(TARGET).elf $(ISO)/boot/$(TARGET).elf
> cp $(GRUB_CFG) $(ISO)/boot/grub/grub.cfg
> grub-mkrescue -o $(ISO)/$(TARGET).iso $(ISO) 2>/dev/null || \
> xorriso -as mkisofs -R -b boot/grub/i386-pc/eltorito.img -no-emul-boot -boot-load-size 4 -boot-info-table -o $(ISO)/$(TARGET).iso $(ISO)

run: clean all
> qemu-system-i386 -cdrom $(ISO)/$(TARGET).iso \
> -m 256M -smp 2 -machine pc -cpu qemu32 \
> -display gtk \
> -serial stdio


clean:
> rm -rf $(BUILD) $(ISO)