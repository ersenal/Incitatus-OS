#!/bin/sh
Linker="/usr/local/cross/bin/i586-elf-ld"
C_Compiler="/usr/local/cross/bin/i586-elf-gcc"
CFlags="-nostdlib -fno-builtin -fno-stack-protector -O0 -Wall -Wextra -Werror -std=gnu99 -I kernel/include/ -I user/include"

#! User Space
$C_Compiler $CFlags -o incit.o   -c   user/src/Lib/Incitatus.c
ar rcs bin/libIncitatus.a incit.o

$C_Compiler $CFlags -o shell.o    -c user/src/Apps/Shell.c
$C_Compiler $CFlags -o ls.o       -c user/src/Apps/ls.c
$C_Compiler $CFlags -o test.o     -c user/src/Apps/test.c
$C_Compiler $CFlags -o restart.o  -c user/src/Apps/restart.c
$C_Compiler $CFlags -o cls.o      -c user/src/Apps/cls.c

$Linker -T user/src/Apps/apps.ld -s -nostdlib -o Shell   shell.o     bin/libIncitatus.a
$Linker -T user/src/Apps/apps.ld -s -nostdlib -o ls      ls.o        bin/libIncitatus.a
$Linker -T user/src/Apps/apps.ld -s -nostdlib -o test    test.o      bin/libIncitatus.a
$Linker -T user/src/Apps/apps.ld -s -nostdlib -o restart restart.o   bin/libIncitatus.a
$Linker -T user/src/Apps/apps.ld -s -nostdlib -o cls     cls.o       bin/libIncitatus.a

tar cvf bootloader/initrd.tar Shell ls test restart cls

rm Shell
rm ls
rm test
rm restart
rm cls
#!

#! Kernel
nasm -f elf -o start.o   kernel/src/Start.s
nasm -f elf -o idtAsm.o  kernel/src/X86/IDT.s

$C_Compiler $CFlags -o kernel.o  -c   kernel/src/Kernel.c
$C_Compiler $CFlags -o sys.o     -c   kernel/src/Sys.c
$C_Compiler $CFlags -o module.o  -c   kernel/src/Module.c

$C_Compiler $CFlags -o vga.o     -c   kernel/src/Drivers/VGA.c
$C_Compiler $CFlags -o console.o -c   kernel/src/Drivers/Console.c
$C_Compiler $CFlags -o ps2.o     -c   kernel/src/Drivers/PS2Controller.c
$C_Compiler $CFlags -o kbd.o     -c   kernel/src/Drivers/Keyboard.c
$C_Compiler $CFlags -o mouse.o   -c   kernel/src/Drivers/Mouse.c

$C_Compiler $CFlags -o gdt.o     -c   kernel/src/X86/GDT.c
$C_Compiler $CFlags -o pic.o     -c   kernel/src/X86/PIC8259.c
$C_Compiler $CFlags -o idt.o     -c   kernel/src/X86/IDT.c
$C_Compiler $CFlags -o pit.o     -c   kernel/src/X86/PIT8253.c
$C_Compiler $CFlags -o cpu.o     -c   kernel/src/X86/CPU.c
$C_Compiler $CFlags -o user.o    -c   kernel/src/X86/Usermode.c

$C_Compiler $CFlags -o bitmap.o  -c   kernel/src/Lib/Bitmap.c
$C_Compiler $CFlags -o stack.o   -c   kernel/src/Lib/Stack.c
$C_Compiler $CFlags -o math.o    -c   kernel/src/Lib/Math.c
$C_Compiler $CFlags -o arrlist.o -c   kernel/src/Lib/ArrayList.c
$C_Compiler $CFlags -o linkl.o   -c   kernel/src/Lib/LinkedList.c
$C_Compiler $CFlags -o string.o  -c   kernel/src/Lib/String.c
$C_Compiler $CFlags -o fifo.o    -c   kernel/src/Lib/CircularFIFOBuffer.c

$C_Compiler $CFlags -o pmm.o     -c   kernel/src/Memory/PhysicalMemory.c
$C_Compiler $CFlags -o smm.o     -c   kernel/src/Memory/StackPMM.c
$C_Compiler $CFlags -o bmm.o     -c   kernel/src/Memory/BitmapPMM.c
$C_Compiler $CFlags -o vmm.o     -c   kernel/src/Memory/VirtualMemory.c
$C_Compiler $CFlags -o heap.o    -c   kernel/src/Memory/HeapMemory.c
$C_Compiler $CFlags -o dl.o      -c   kernel/src/Memory/DougLea.c
$C_Compiler $CFlags -o dumbH.o   -c   kernel/src/Memory/DumbHeapManager.c

$C_Compiler $CFlags -o sched.o   -c   kernel/src/Process/Scheduler.c
$C_Compiler $CFlags -o rr.o      -c   kernel/src/Process/RoundRobin.c
$C_Compiler $CFlags -o fcfs.o    -c   kernel/src/Process/FCFS.c
$C_Compiler $CFlags -o pm.o      -c   kernel/src/Process/ProcessManager.c

$C_Compiler $CFlags -o ramdisk.o -c   kernel/src/FileSystem/RamDisk.c
$C_Compiler $CFlags -o tar.o     -c   kernel/src/FileSystem/Tar.c
$C_Compiler $CFlags -o vfs.o     -c   kernel/src/FileSystem/VFS.c

$Linker -Map bin/Mem.map -T kernel/src/Linker.ld -o bootloader/kernel   start.o \
                                                                        kernel.o \
                                                                        vga.o \
                                                                        console.o \
                                                                        sys.o \
                                                                        module.o \
                                                                        gdt.o \
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
                                                                        dumbH.o \
                                                                        linkl.o \
                                                                        string.o \
                                                                        fifo.o \
                                                                        sched.o \
                                                                        rr.o \
                                                                        fcfs.o \
                                                                        pm.o \
                                                                        kbd.o \
                                                                        mouse.o \
                                                                        ps2.o \
                                                                        ramdisk.o \
                                                                        tar.o \
                                                                        vfs.o \
                                                                        user.o \


genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o bin/image.iso bootloader/

rm *.o
rm bootloader/kernel
