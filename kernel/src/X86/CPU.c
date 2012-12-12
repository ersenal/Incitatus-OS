/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| CPU.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Provides functions to alter or retrieve CPU control
|               registers. Control registers(CR0..CR4) control the general
|               function of CPU.
|
| EXAMPLE:      You may use FORCE_CAST(refer to Common.h) to modify
|               or read the control registers:
|
|               u32int tmp = CPU_getCR(0);
|               CR0 cr0 = FORCE_CAST(tmp, CR0);
|               cr0.PG = 1;
|               CPU_setCR(0, FORCE_CAST(cr0, u32int));
|
| NOTES:        Control registers are 32-bit registers(on X86-32).
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <X86/CPU.h>

PUBLIC u32int CPU_getCR(u8int n) {

    u32int ret = 0;

    switch(n) {

        case 0:
        asm volatile("mov %%cr0, %0 " : "=a" (ret));
        break;

        case 1:
        asm volatile("mov %%cr1, %0 " : "=a" (ret));
        break;

        case 2:
        asm volatile("mov %%cr2, %0 " : "=a" (ret));
        break;

        case 3:
        asm volatile("mov %%cr3, %0 " : "=a" (ret));
        break;

        case 4:
        asm volatile("mov %%cr4, %0 " : "=a" (ret));
        break;

    }

    return ret;
}

PUBLIC void CPU_setCR(u8int n, u32int val) {

    switch(n) {

        case 0:
        asm volatile("mov %0, %%cr0" : : "a" (val));
        break;

        case 1:
        asm volatile("mov %0, %%cr1" : : "a" (val));
        break;

        case 2:
        asm volatile("mov %0, %%cr2" : : "a" (val));
        break;

        case 3:
        asm volatile("mov %0, %%cr3" : : "a" (val));
        break;

        case 4:
        asm volatile("mov %0, %%cr4" : : "a" (val));
        break;

    }

}

