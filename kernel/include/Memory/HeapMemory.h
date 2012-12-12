/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| HeapMemory.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Heap memory manager interface.
|               Sets up and manages heap memory.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef HEAP_H
#define HEAP_H

#include <Module.h>

/*=======================================================
    DEFINE
=========================================================*/
#define KERNEL_HEAP_VADDR 0x40000000 /* Kernel heap starts at 1GB virtual address*/

/*=======================================================
    FUNCTION
=========================================================*/
void* HeapMemory_expand(u32int size);
void HeapMemory_contract(u32int size);
Module* HeapMemory_getModule(void);
#endif