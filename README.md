# vm_dos — minimal bootable OS in C

A bare-metal x86 operating system kernel written entirely in C and assembly, booting via GRUB Multiboot. Designed as an educational platform for learning OS fundamentals with a working shell, filesystem abstraction, and interrupt handling.

## Overview

vm_dos provides a minimal but functional kernel environment including:

- **Boot**: GRUB-based Multiboot protocol entry point
- **CPU State**: GDT (Global Descriptor Table) and IDT (Interrupt Descriptor Table) setup
- **Interrupts**: ISR/IRQ handlers with PIC remapping for hardware interrupts
- **Timing**: PIT (Programmable Interval Timer) for system ticks and sleep functionality
- **Memory**: PMM (Physical Memory Manager) with bitmap allocator and identity paging
- **I/O**: VGA text console with color support, keyboard driver with line-buffering
- **Filesystem**: Minimal VFS layer with ramfs backend for in-kernel file storage
- **Userland**: Kernel-embedded init system and interactive shell with command parsing
- **UI**: Themed console layout with toast notifications and progress animations

## Prerequisites

### Toolchain
- **i686-elf-gcc** — 32-bit ELF GCC cross-compiler
- **i686-elf-ld** — 32-bit ELF linker
- **i686-elf-as** — 32-bit assembler (GNU as)

### Utilities
- **grub-mkrescue** — ISO generation tool (or xorriso as fallback)
- **xorriso** — ISO9660 creation utility
- **qemu-system-i386** — x86 system emulator

### Installation (Linux/WSL)
```bash
# Install cross-compiler and tools
sudo apt-get install gcc-i686-linux-gnu binutils-i386-linux-gnu
sudo apt-get install grub-pc-bin xorriso
sudo apt-get install qemu-system-x86
```

## Build and Run

```bash
# Full clean rebuild and boot
make run

# Build kernel only
make

# Clean build artifacts
make clean
```

The `make run` target rebuilds from scratch and automatically launches the kernel in QEMU.

## Architecture

### Directory Structure
```
vm_dos/
├── kernel/               # Kernel core
│   ├── arch/x86/        # CPU-specific (GDT, IDT, ISR, paging, PMM)
│   ├── lib/             # Utilities (string.h, etc.)
│   ├── shell/           # Interactive shell
│   ├── ui/              # Console UI (theme, layout, animations)
│   └── kernel.c         # Main kernel entry (kmain)
├── fs/                  # Filesystem layer (VFS, ramfs)
├── user/                # Userland (init process)
├── boot/                # GRUB config
├── iso/                 # ISO build staging
├── build/               # Compiled objects and final ELF
├── Makefile
├── linker.ld            # Linker script (memory layout)
└── multiboot.s          # Multiboot header stub
```

### Boot Flow
1. GRUB loads `multiboot.s` (Multiboot protocol header)
2. Bootloader jumps to `kernel/arch/x86/boot.S` (stack setup, enable paging)
3. Control passes to `kmain()` in `kernel.c`
4. System initialization: GDT → IDT → ISRs → PIC → PIT → PMM → Paging
5. Userland init starts; shell loop begins

### Memory Layout
- **0x00100000** — Kernel load address (1 MB, typical Multiboot location)
- **Identity mapping** — Virtual = Physical (simplified paging)
- **Stack** — Grows downward from high memory
- **Heap** — Managed by PMM (bitmap-based)

## Features in Detail

### Interrupt Handling
- **ISR Table** — 32 CPU exceptions (faults, traps, aborts)
- **IRQ Routing** — 8 legacy IRQs remapped via PIC
- **Keyboard** — IRQ1 handler reads scan codes; line-buffering with backspace support
- **PIT Timer** — IRQ0 fires at 100 Hz; used for `pit_sleep()` and animations

### Console & UI
- **VGA Mode** — 80×25 text color (16-color palette)
- **Console API** — `con_write()`, `con_write_at()`, `con_putc_at()`, `con_clear()`, `con_box()`
- **Theme** — Single default color scheme (foreground, background, accent, status indicators)
- **Animations** — Toast notifications, progress bars, startup animation
- **Layout** — Chrome frame, command input area, status line

### Shell Commands
- `help` — List available commands
- `clear` — Clear screen and redraw UI
- `echo <text>...` — Print text to console
- `panel` — Redraw UI chrome
- `progress` — Animated progress bar demo (40-char width, 4-second duration)
- `ls [path]` — List files in filesystem (default: `/`)
- `cat <path>` — Display file contents
- `toast <message>` — Show temporary notification

### Filesystem (VFS)
- **ramfs Backend** — All files stored in kernel memory as a simple array
- **Paths** — Unix-style paths (`/`, `/bin/init.txt`)
- **Init File** — Embedded text file at `/bin/init.txt` (can be edited in `fs/initrd.c`)
- **Limitations** — Read-only, no directories, flat namespace

### Physical Memory Manager
- **Bitmap Allocator** — One bit per 4 KB page frame
- **API** — `pmm_alloc()`, `pmm_free()`, `pmm_init()`
- **Scope** — Manages kernel heap; not for user processes (yet)

## Compilation & Linking

The Makefile compiles each `.c` file to object files, assembles `.S` (uppercase) assembly, and links all objects with `linker.ld`. Key flags:
- `-m32` — Generate 32-bit x86 code
- `-ffreestanding` — No libc, no runtime support
- `-nostdlib -nostdinc` — Exclude standard library headers
- `-fno-pic -fno-pie` — No position-independent code (kernel must load at fixed address)

## Development & Customization

### Adding a Shell Command
1. Write command function in `kernel/shell/commands.c`:
   ```c
   static int cmd_mycommand(int argc, const char **argv) {
       con_write_at(25, 10, "Hello from my command!");
       return 0;
   }
   ```
2. Add entry to `COMMANDS[]` array:
   ```c
   {"mycommand", "My help text", cmd_mycommand},
   ```
3. Rebuild with `make run`

### Modifying Console Output
- Edit colors in `kernel/ui/theme.c` (default theme structure)
- Redraw chrome in `kernel/ui/layout.c` (`ui_draw_chrome()`)
- Change startup message and banner in `kernel/kernel.c`

### Adding Files to Filesystem
- Edit `fs/initrd.c` to add more entries to the ramfs
- Update path references in shell or userland code
- No dynamic file creation yet (filesystem is static)

## Known Limitations & TODOs

### Current Constraints
- **Single theme** — No runtime theming; compile-time only
- **No real userspace** — Init and shell run in kernel mode (no privilege separation)
- **Read-only filesystem** — ramfs is static; no write/delete operations
- **Limited keyboard** — ASCII printables only; no modifier keys or function keys
- **Basic PMM** — Bitmap-based; no buddy allocator or sophisticated strategies
- **No virtual memory** — Identity paging only; no demand paging or swapping

### Roadmap
- [ ] Proper userspace with ring 3 execution
- [ ] ELF loader for dynamic programs
- [ ] Real process management (scheduling, context switching)
- [ ] Writable filesystem (tmpfs, ext2)
- [ ] System call interface (gate, argument passing, return values)
- [ ] PCI enumeration and driver framework
- [ ] ATA/AHCI disk controller support
- [ ] Networking stack (if disk support is added)

## Troubleshooting

### Compilation Errors
- **"unrecognized command-line option '--m32'"** — Use `-m32` (single dash), not `--m32`
- **"missing .note.GNU-stack"** — Assembly warning; can be ignored. Add `.section .note.GNU-stack, "", %progbits` to `.S` files to suppress.

### Runtime Issues
- **QEMU window shows nothing** — Check terminal output; kernel may have panicked. Look for early boot messages.
- **Shell doesn't respond to input** — Keyboard driver may not be initialized. Verify IRQ1 is unmasked after PIC remap.
- **File not found** — ramfs paths must match entries in `fs/initrd.c`. Use `ls` to debug.

### Building on Windows
- Use WSL2 with Ubuntu; install cross-compiler in WSL
- Run `make run` from WSL terminal
- QEMU can display to X11 forwarding or use `-display none` and `-serial stdio` for serial output only

## Notes

- **No ANSI Escape Sequences** — VGA text mode doesn't interpret escape codes. Use console API functions instead.
- **Bare Metal** — No libc means no `printf()`, `malloc()`, standard I/O. Everything is custom.
- **Educational Focus** — Code prioritizes clarity over performance; not suitable for production.
- **x86-32 Only** — Currently targets i686 (Pentium+). x86-64 support would require significant changes.

## License
Public domain / MIT — your choice. Use freely for learning, experimentation, and iteration.
