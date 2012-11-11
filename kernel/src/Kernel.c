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

PUBLIC void Kernel(void) {

    Module* modules[] = {

        VGA_getModule(),
        Console_getModule(),
        GDT_getModule(),
        PIC8259_getModule(),
        IDT_getModule(),
        PIT8253_getModule()

    };

    for(u32int i = 0; i < ARRAY_SIZE(modules); i++)
        Module_load(modules[i]);

    Console_clearScreen();

    while(1) Sys_haltCPU();

}

