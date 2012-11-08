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
#include <Debug.h>
#include <Sys.h>
#include <X86/GDT.h>
#include <X86/IDT.h>

PUBLIC void Kernel(void) {

    Module_load(VGA_getModule());
    Module_load(Console_getModule());
    Module_load(GDT_getModule());
    Module_load(IDT_getModule());

    Console_clearScreen();
    Console_setColor(CONSOLE_INFO);
    Console_printString("Loaded Modules:\n");
    Console_setColor(CONSOLE_NORMAL);

    char* moduleNames[MAX_LOADED_MODULES];
    Module_getLoadedModuleNames(moduleNames);

    for(u32int i = 0; i < Module_getNumberOfLoadedModules(); i++)
        Console_printf("%s%c", moduleNames[i], '\n');


    while(1) Sys_haltCPU();

}

