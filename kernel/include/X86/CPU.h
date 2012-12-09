/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| CPU.h
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
|               CR0 cr0 = FORCE_CAST(temp, CR0);
|               cr0.PG = 1;
|               CPU_setCR(0, FORCE_CAST(cr0, u32int));
|
| NOTES:        Control registers are 32-bit registers(on X86-32).
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef CPU_H
#define CPU_H

#include <Common.h>

/*=======================================================
    STRUCT
=========================================================*/
typedef struct CR0 CR0;
typedef struct CR3 CR3;

struct CR0 {

    u8int PE :  1; /* 0: real mode, 1: protected mode */
    u8int MP :  1; /* Monitor co-processor */
    u8int EM :  1; /* 0: x87 FPU present, 1: x87 FPU NOT present */
    u8int TS :  1; /* Task switched */
    u8int ET :  1; /* Extension type */
    u8int NE :  1; /* Numeric error */
    u16int   : 10;
    u8int WP :  1; /* Write protect */
    u8int    :  1;
    u8int AM :  1; /* Alignment mask */
    u16int   : 10;
    u8int NW :  1; /* Enable/Disable write-back caching */
    u8int CD :  1; /* Enable/Disable memory cache */
    u8int PG :  1; /* 0: disable paging, 1: enable paging */

} __attribute__((packed));

struct CR3 {

    u16int      : 12;
    u32int PDBR : 20; /* Page directory base register */

} __attribute__((packed));

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Get control register n(0..4)
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the contents of the specified control register.
|
| PARAM:           "n"  "n"th control register
|
| PRECONDITION:    "n"  needs to be 1..4
|
| RETURN:          u32int   contents of the "n"th control register
\------------------------------------------------------------------------*/
u32int CPU_getCR(u8int n);

/*-------------------------------------------------------------------------
| Set control register n(0..4)
|--------------------------------------------------------------------------
| DESCRIPTION:     Alters the contents of the specified control register.
|
| PARAM:           "n"    "n"th control register
|                  "val"  new contents of the "n"th control register
|
| PRECONDITION:    "n"   needs to be 1..4
\------------------------------------------------------------------------*/
void CPU_setCR(u8int n, u32int val);
#endif