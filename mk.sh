#!/bin/sh
Linker="/usr/local/cross/bin/i586-elf-ld"
C_Compiler="/usr/local/cross/bin/i586-elf-gcc"
CFlags="-nostdlib -fno-builtin -fno-stack-protector -O2 -Wall -Wextra -Werror -std=gnu99 -I kernel/include/"

nasm -f elf -o start.o   kernel/src/Start.s
nasm -f elf -o gdtAsm.o  kernel/src/X86/GDT.s
nasm -f elf -o idtAsm.o  kernel/src/X86/IDT.s

$C_Compiler $CFlags -o kernel.o  -c   kernel/src/Kernel.c
$C_Compiler $CFlags -o io.o      -c   kernel/src/IO.c
$C_Compiler $CFlags -o sys.o     -c   kernel/src/Sys.c
$C_Compiler $CFlags -o module.o  -c   kernel/src/Module.c
$C_Compiler $CFlags -o mem.o     -c   kernel/src/Memory.c

$C_Compiler $CFlags -o vga.o     -c   kernel/src/Drivers/VGA.c
$C_Compiler $CFlags -o console.o -c   kernel/src/Drivers/Console.c

$C_Compiler $CFlags -o gdt.o     -c   kernel/src/X86/GDT.c
$C_Compiler $CFlags -o pic.o     -c   kernel/src/X86/PIC8259.c
$C_Compiler $CFlags -o idt.o     -c   kernel/src/X86/IDT.c
$C_Compiler $CFlags -o pit.o     -c   kernel/src/X86/PIT8253.c
$C_Compiler $CFlags -o cpu.o     -c   kernel/src/X86/CPU.c

$C_Compiler $CFlags -o bitmap.o  -c   kernel/src/Lib/Bitmap.c
$C_Compiler $CFlags -o stack.o   -c   kernel/src/Lib/Stack.c
$C_Compiler $CFlags -o math.o    -c   kernel/src/Lib/Math.c
$C_Compiler $CFlags -o arrlist.o -c   kernel/src/Lib/ArrayList.c

$C_Compiler $CFlags -o pmm.o     -c   kernel/src/Memory/PhysicalMemory.c
$C_Compiler $CFlags -o smm.o     -c   kernel/src/Memory/StackPMM.c
$C_Compiler $CFlags -o bmm.o     -c   kernel/src/Memory/BitmapPMM.c
$C_Compiler $CFlags -o vmm.o     -c   kernel/src/Memory/VirtualMemory.c
$C_Compiler $CFlags -o heap.o    -c   kernel/src/Memory/HeapMemory.c
$C_Compiler $CFlags -o dl.o      -c   kernel/src/Memory/DougLea.c

$Linker -Map bin/Mem.map -T kernel/src/Linker.ld -o bootloader/kernel   start.o \
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
                                                                        stack.o \
                                                                        math.o \
                                                                        pmm.o \
                                                                        smm.o \
                                                                        bmm.o \
                                                                        vmm.o \
                                                                        heap.o \
                                                                        dl.o \
                                                                        arrlist.o \


genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o bin/image.iso bootloader/

rm *.o
rm bootloader/kernel
