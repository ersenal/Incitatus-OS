/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| BitmapPMM.h (implements PhysicalMemory)
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Bitmap based physical memory manager implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef BIT_PMM
#define BIT_PMM

#include <Multiboot.h>

/*-------------------------------------------------------------------------
| Allocate frame
|--------------------------------------------------------------------------
| DESCRIPTION:     Allocates and returns a frame address(4096 bytes).
|
| RETURN:          void*  allocated frame address
|
| NOTES:           Returns NULL if out of physical memory.
\------------------------------------------------------------------------*/
void* BitmapPMM_allocateFrame(void);

/*-------------------------------------------------------------------------
| Free frame
|--------------------------------------------------------------------------
| DESCRIPTION:     Deallocates the frame at address "b".
|
| PARAM:           "b"  the address of frame to be deallocated
\------------------------------------------------------------------------*/
void BitmapPMM_freeFrame(void* b);

/*-------------------------------------------------------------------------
| Physical memory initialisation
|--------------------------------------------------------------------------
| DESCRIPTION:     Sets up physical memory.
|
| PARAM:          "mbI"  points to multiboot information
|                 "mbH"  points to multiboot header
|                 (refer to Multiboot.h)
\------------------------------------------------------------------------*/
void BitmapPMM_init(MultibootInfo* mbI, MultibootHeader* mbH);

/*-------------------------------------------------------------------------
| Print current memory information
|--------------------------------------------------------------------------
| DESCRIPTION:     Prints out physical memory information.
|
\------------------------------------------------------------------------*/
void BitmapPMM_printInfo(void);

#endif