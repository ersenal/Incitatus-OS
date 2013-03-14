/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Kernel.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Main C entry point for kernel, Kernel() function gets
|               called in Start.s.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Drivers/Console.h>
#include <Drivers/VGA.h>
#include <Sys.h>
#include <X86/GDT.h>
#include <X86/PIC8259.h>
#include <X86/IDT.h>
#include <X86/PIT8253.h>
#include <Multiboot.h>
#include <Debug.h>
#include <Memory/PhysicalMemory.h>
#include <Memory/VirtualMemory.h>
#include <Memory/HeapMemory.h>
#include <Process/Scheduler.h>
#include <Drivers/PS2Controller.h>
#include <FileSystem/VFS.h>
#include <X86/Usermode.h>
#include <Memory.h>

PUBLIC MultibootInfo* multibootInfo;

PUBLIC void Kernel(MultibootInfo* mbInfo, u32int mbMagic) {

    extern MultibootHeader mbHead;
    multibootInfo = mbInfo;

    Debug_assert(mbMagic == MULTIBOOT_BOOTLOADER_MAGIC);
    Debug_assert(mbHead.magic == MULTIBOOT_HEADER_MAGIC);
    Debug_assert(mbInfo->modsCount > 0); /* Make sure initrd(ram disk) is in memory */

    Module* modules[] = {

        VGA_getModule(),
        Console_getModule(),
        GDT_getModule(),
        PIC8259_getModule(),
        IDT_getModule(),
        PIT8253_getModule(),
        PhysicalMemory_getModule(),
        VirtualMemory_getModule(),
        HeapMemory_getModule(),
        PS2Controller_getModule(),
        VFS_getModule(),
        Scheduler_getModule(),
        Usermode_getModule(),

    };

    for(u32int i = 0; i < ARRAY_SIZE(modules); i++)
        Module_load(modules[i]);

    Sys_enableInterrupts();
    Sys_panic("Should not reach here!");

}

