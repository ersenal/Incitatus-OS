/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| PhysicalMemory.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Physical memory manager interface.
|               Sets up and manages physical memory.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef PM_H
#define PM_H

#include <Multiboot.h>
#include <Module.h>

/*=======================================================
    DEFINE
=========================================================*/
#define FRAME_SIZE 4096

/*=======================================================
    STRUCT
=========================================================*/
typedef struct PhysicalMemoryInfo PhysicalMemoryInfo;

struct PhysicalMemoryInfo {

    u32int totalMemory; /* Total physical memory in bytes */
    u32int totalFrames; /* Number of page frames */
    u32int freeFrames; /* Number of free(not in use) page frames */

};

/*=======================================================
    INTERFACE
=========================================================*/

/*-------------------------------------------------------------------------
| Physical memory initialisation
|--------------------------------------------------------------------------
| DESCRIPTION:     Sets up physical memory.
|
| PARAM:          "mbI"  points to multiboot information
|                 "mbH"  points to multiboot header
|                 (refer to Multiboot.h)
\------------------------------------------------------------------------*/
extern void (*PhysicalMemory_init) (MultibootInfo* mbI, MultibootHeader* mbH);

/*-------------------------------------------------------------------------
| Get current memory information
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns a structure containing physical memory info.
|
\------------------------------------------------------------------------*/
extern PhysicalMemoryInfo  (*PhysicalMemory_getInfo) (void);

/*-------------------------------------------------------------------------
| Allocate frame
|--------------------------------------------------------------------------
| DESCRIPTION:     Allocates and returns a frame address(4096 bytes).
|
| RETURN:          void*  allocated frame address
|
| NOTES:           Returns NULL if out of physical memory.
\------------------------------------------------------------------------*/
extern void* (*PhysicalMemory_allocateFrame) (void);

/*-------------------------------------------------------------------------
| Free frame
|--------------------------------------------------------------------------
| DESCRIPTION:     Deallocates the frame at address "b".
|
| PARAM:           "b"  the address of frame to be deallocated
\------------------------------------------------------------------------*/
extern void  (*PhysicalMemory_freeFrame) (void* frame);

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Get PMM module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the physical memory management module.
|
\------------------------------------------------------------------------*/
Module* PhysicalMemory_getModule(void);

#endif