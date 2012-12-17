/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| StackPMM.h (implements PhysicalMemory)
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Stack based physical memory manager implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef STACK_PMM
#define STACK_PMM

#include <Multiboot.h>
#include <Memory/PhysicalMemory.h>

/*-------------------------------------------------------------------------
| Allocate frame
|--------------------------------------------------------------------------
| DESCRIPTION:     Allocates and returns a frame address(4096 bytes).
|
| RETURN:          void*  allocated frame address
|
| NOTES:           Returns NULL if out of physical memory.
\------------------------------------------------------------------------*/
void* StackPMM_allocateFrame(void);

/*-------------------------------------------------------------------------
| Free frame
|--------------------------------------------------------------------------
| DESCRIPTION:     Deallocates the frame at address "b".
|
| PARAM:           "b"  the address of frame to be deallocated
\------------------------------------------------------------------------*/
void StackPMM_freeFrame(void* b);

/*-------------------------------------------------------------------------
| Physical memory initialisation
|--------------------------------------------------------------------------
| DESCRIPTION:     Sets up physical memory.
|
| PARAM:          "mbI"  points to multiboot information
|                 "mbH"  points to multiboot header
|                 (refer to Multiboot.h)
\------------------------------------------------------------------------*/
void StackPMM_init(MultibootInfo* mbI, MultibootHeader* mbH);

/*-------------------------------------------------------------------------
| Get current memory information
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns a structure containing physical memory info.
|
\------------------------------------------------------------------------*/
PhysicalMemoryInfo  StackPMM_getInfo(void);

#endif