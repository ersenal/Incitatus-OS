/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| DumbHeapManager.c (implements HeapMemory)
|--------------------------------------------------------------------------
|
| DESCRIPTION:  A useless heap manager implementation, only supports malloc.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Memory/DumbHeapManager.h>
#include <Memory/VirtualMemory.h>
#include <Memory/HeapMemory.h>
#include <Memory/PhysicalMemory.h>
#include <Debug.h>
#include <Memory.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE char*  lastAllocAddr = (char*) KERNEL_HEAP_BASE_VADDR;
PRIVATE u32int allocatedPages;

/*=======================================================
    FUNCTION
=========================================================*/

PUBLIC void* DumbHeapManager_malloc(size_t bytes) {

    /* out of heap memory? */
    if(((lastAllocAddr - ((char*) KERNEL_HEAP_BASE_VADDR) + bytes) / FRAME_SIZE) + 1 > allocatedPages) {

        HeapMemory_expand(FRAME_SIZE);
        allocatedPages++;

    }

    void* temp = lastAllocAddr;
    lastAllocAddr += bytes;
    return temp;

}

PUBLIC void* DumbHeapManager_realloc(void* oldmem, size_t bytes) {

    /* This manager does not implement reallocate */
    UNUSED(oldmem);
    UNUSED(bytes);
    Debug_assert(FALSE);
    return NULL;

}

PUBLIC void* DumbHeapManager_calloc(size_t numberOfElements, size_t elementSize) {

    /* This manager does not implement calloc */
    UNUSED(numberOfElements);
    UNUSED(elementSize);
    Debug_assert(FALSE);
    return NULL;

}

PUBLIC void DumbHeapManager_free(void* mem) {

    /* This manager does not implement free */
    UNUSED(mem);
    Debug_assert(FALSE);

}
