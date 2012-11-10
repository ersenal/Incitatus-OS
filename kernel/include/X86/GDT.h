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

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Get GDT module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the GDT module.
|
\------------------------------------------------------------------------*/
Module* GDT_getModule(void);
#endif
