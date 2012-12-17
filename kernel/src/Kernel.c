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
|               called by Start.s.
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
#include <Process/ProcessManager.h>


PUBLIC void Kernel(MultibootInfo* mbInfo, u32int mbMagic) {

    extern MultibootHeader mbHead;

    Debug_assert(mbMagic == MULTIBOOT_BOOTLOADER_MAGIC);
    Debug_assert(mbHead.magic == MULTIBOOT_HEADER_MAGIC);

    Module_load(VGA_getModule());
    Module_load(Console_getModule());
    Console_clearScreen();

    Module* modules[] = {

        GDT_getModule(),
        PIC8259_getModule(),
        IDT_getModule(),
        PIT8253_getModule(),
        PhysicalMemory_getModule(mbInfo, &mbHead),
        VirtualMemory_getModule(),
        HeapMemory_getModule(),
        ProcessManager_getModule()

    };

    for(u32int i = 0; i < ARRAY_SIZE(modules); i++)
        Module_load(modules[i]);

    asm volatile("sti");

    while(1) {

        Console_clearScreen();
        Sys_haltCPU();
    }
}

