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

PUBLIC void Kernel(void) {

    Module_load(VGA_getModule());
    Module_load(Console_getModule());

    Console_clearScreen();

    for(int row = 0; row < 50; row++)
            Console_printf("%s%d%c", "Line ", row, '\n');

    while(1) Sys_haltCPU();

}

