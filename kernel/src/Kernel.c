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
#include <Debug.h>

void Kernel(void) {

    Console_init();
    Console_clearScreen();

    Debug_logInfo("%s", "Hello World!");
    Sys_panic("Goodbye World!");

    while(1) asm volatile("hlt");

}

