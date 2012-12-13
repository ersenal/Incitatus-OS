/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| DumbHeapManager.h (implements HeapMemory)
|--------------------------------------------------------------------------
|
| DESCRIPTION:  A useless heap manager implementation, only supports malloc.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef DHM_H
#define DHM_H

#include <Module.h>
#include <Common.h>

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
void* DumbHeapManager_malloc(size_t bytes);

/*-------------------------------------------------------------------------
| Heap reallocation (Not implemented)
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
void* DumbHeapManager_realloc(void* oldmem, size_t bytes);

/*-------------------------------------------------------------------------
| Heap clear allocation (Not implemented)
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
void* DumbHeapManager_calloc(size_t numberOfElements, size_t elementSize);

/*-------------------------------------------------------------------------
| Heap free (Not implemented)
|--------------------------------------------------------------------------
| DESCRIPTION:     Deallocates the specified memory block.
|
| PARAM:           "mem"  pointer to memory block
\------------------------------------------------------------------------*/
void  DumbHeapManager_free(void* mem);

#endif