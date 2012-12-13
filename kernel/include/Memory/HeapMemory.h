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
#include <Common.h>

/*=======================================================
    DEFINE
=========================================================*/
#define KERNEL_HEAP_VADDR 0x20000000 /* Kernel heap starts at 512MB virtual address*/

/*=======================================================
    INTERFACE
=========================================================*/

/*-------------------------------------------------------------------------
| Heap allocation
|--------------------------------------------------------------------------
| DESCRIPTION:     Allocates "bytes" number of bytes of space in heap
|                  and returns the allocated address.
|
| PARAM:           "bytes"  number of bytes to allocate
|
| RETURN:          void*   pointer to allocated space
|
\------------------------------------------------------------------------*/
extern void* (*HeapMemory_alloc)   (size_t bytes);

/*-------------------------------------------------------------------------
| Heap reallocation
|--------------------------------------------------------------------------
| DESCRIPTION:     Reallocates the given memory block to a new block of
|                  size "bytes".
|
| PARAM:           "oldmem"   pointer to old memory block
|                  "bytes"    new size of memory block in bytes
|
| RETURN:          void*   pointer to reallocated space
|
\------------------------------------------------------------------------*/
extern void* (*HeapMemory_realloc) (void* oldmem, size_t bytes);

/*-------------------------------------------------------------------------
| Heap clear allocation
|--------------------------------------------------------------------------
| DESCRIPTION:     Allocates an array of "numberOfElements" elements and
|                  sets all bits as zero. Each element has "elementSize"
|                  length.
|
| PARAM:           "numberOfElements" number of elements to be allocated
|                  "elementSize"      size of an element in bytes
|
| RETURN:          void*   pointer to cleared and allocated space
|
\------------------------------------------------------------------------*/
extern void* (*HeapMemory_calloc)  (size_t numberOfElements, size_t elementSize);

/*-------------------------------------------------------------------------
| Heap free
|--------------------------------------------------------------------------
| DESCRIPTION:     Deallocates the specified memory block.
|
| PARAM:           "mem"  pointer to memory block
\------------------------------------------------------------------------*/
extern void  (*HeapMemory_free)    (void* mem);

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Heap expand
|--------------------------------------------------------------------------
| DESCRIPTION:     Expands the heap space by "size" bytes. "size" needs to
|                  be page aligned.
|
| PARAM:           "size"  Page aligned number of bytes
\------------------------------------------------------------------------*/
void*   HeapMemory_expand(ptrdiff_t size);

/*-------------------------------------------------------------------------
| Heap contract
|--------------------------------------------------------------------------
| DESCRIPTION:     Contracts the heap space by "size" bytes. "size" needs to
|                  be page aligned.
|
| PARAM:           "size"  Page aligned number of bytes
\------------------------------------------------------------------------*/
void    HeapMemory_contract(size_t size);

/*-------------------------------------------------------------------------
| Get kernel heap manager module
|--------------------------------------------------------------------------
| DESCRIPTION:    Returns the kernel heap memory management module.
|
\------------------------------------------------------------------------*/
Module* HeapMemory_getModule(void);

#endif