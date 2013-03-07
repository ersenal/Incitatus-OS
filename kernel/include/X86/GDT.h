/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| GDT.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Sets up Global Descriptor Table.
|
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef GDT_H
#define GDT_H

#include <Module.h>

/*=======================================================
    DEFINE
=========================================================*/

/* Segment selectors */
#define NULL_SEGMENT        0x00
#define KERNEL_CODE_SEGMENT 0x08 /* Base 0, Limit 4GB, Privilege 0 */
#define KERNEL_DATA_SEGMENT 0x10 /* Base 0, Limit 4GB, Privilege 0 */
#define USER_CODE_SEGMENT   0x18 /* Base 0, Limit 4GB, Privilege 3 */
#define USER_DATA_SEGMENT   0x20 /* Base 0, Limit 4GB, Privilege 3 */
#define TSS_SEGMENT         0x28 /* Task state segment selector */

 /* Privilege levels */
#define KERNEL_MODE  0
#define USER_MODE    3

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Set TSS
|--------------------------------------------------------------------------
| DESCRIPTION:     Sets the task state segment to specified values.
| //TODO: add param info
\------------------------------------------------------------------------*/
void GDT_setTSS(u32int dataSegment, u32int esp0);

/*-------------------------------------------------------------------------
| Get GDT module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the GDT module.
|
\------------------------------------------------------------------------*/
Module* GDT_getModule(void);
#endif
