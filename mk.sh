#!/bin/sh
CFlags="-nostdlib -fno-builtin -fno-stack-protector -O2 -Wall -Wextra -Werror -std=gnu99 -I kernel/include/"

nasm -f elf -o start.o   kernel/src/Start.s
nasm -f elf -o gdtAsm.o  kernel/src/X86/GDT.s
nasm -f elf -o idtAsm.o  kernel/src/X86/IDT.s

/usr/local/cross/bin/i586-elf-gcc $CFlags -o kernel.o  -c   kernel/src/Kernel.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o io.o      -c   kernel/src/IO.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o vga.o     -c   kernel/src/Drivers/VGA.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o console.o -c   kernel/src/Drivers/Console.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o sys.o     -c   kernel/src/Sys.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o module.o  -c   kernel/src/Module.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o mem.o     -c   kernel/src/Memory.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o gdt.o     -c   kernel/src/X86/GDT.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o pic.o     -c   kernel/src/X86/PIC8259.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o idt.o     -c   kernel/src/X86/IDT.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o pit.o     -c   kernel/src/X86/PIT8253.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o cpu.o     -c   kernel/src/X86/CPU.c

/usr/local/cross/bin/i586-elf-gcc $CFlags -o bitmap.o  -c   kernel/src/Lib/Bitmap.c
/usr/local/cross/bin/i586-elf-gcc $CFlags -o math.o  -c     kernel/src/Lib/Math.c

/usr/local/cross/bin/i586-elf-ld -Map bin/Mem.map -T kernel/src/Linker.ld -o bootloader/kernel        start.o \
                                                                                                      kernel.o \
                                                                                                      io.o \
                                                                                                      vga.o \
                                                                                                      console.o \
                                                                                                      sys.o \
                                                                                                      module.o \
                                                                                                      mem.o \
                                                                                                      gdt.o \
                                                                                                      gdtAsm.o \
                                                                                                      pic.o \
                                                                                                      idt.o \
                                                                                                      idtAsm.o \
                                                                                                      pit.o \
                                                                                                      cpu.o \
                                                                                                      bitmap.o \
                                                                                                      math.o \


genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o bin/image.iso bootloader/

rm *.o
rm bootloader/kernel
