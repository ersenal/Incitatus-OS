/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| HeapMemory.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Heap memory manager interface.
|               Sets up and manages heap memory.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Memory/HeapMemory.h>
#include <Memory/VirtualMemory.h>
#include <Memory/PhysicalMemory.h>
#include <Memory.h>
#include <Debug.h>

/* Include Heap manager implementation */
#include <Memory/DougLea.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module heapModule;
PRIVATE char*  heapTop;

/*=======================================================
    PUBLIC DATA
=========================================================*/
PUBLIC void* (*HeapMemory_alloc)   (size_t bytes);
PUBLIC void* (*HeapMemory_realloc) (void* oldmem, size_t bytes);
PUBLIC void* (*HeapMemory_calloc)  (size_t numberOfElements, size_t elementSize);
PUBLIC void  (*HeapMemory_free)    (void* mem);

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void HeapMemory_init(void) {

    heapTop = (void*) KERNEL_HEAP_VADDR;

    /* Point to memory manager implementation */
    HeapMemory_alloc   = &DougLea_malloc;
    HeapMemory_realloc = &DougLea_realloc;
    HeapMemory_calloc  = &DougLea_calloc;
    HeapMemory_free    = &DougLea_free;

}

PUBLIC void* HeapMemory_expand(ptrdiff_t size) {

    Debug_assert(size % FRAME_SIZE == 0);

    /* The number of needed pages */
    u32int pages = size / FRAME_SIZE;

    void* ret = heapTop;

    for(u32int i = 0; i < pages; i++) {

        void* physicalAddress = PhysicalMemory_allocateFrame();
        Debug_assert(physicalAddress != NULL);
        VirtualMemory_mapPage(heapTop, physicalAddress);
        //Memory_set(heapTop, 0, FRAME_SIZE); /* Let CALLOC handle this */
        heapTop += FRAME_SIZE;

    }

    return ret;
}

PUBLIC void HeapMemory_contract(u32int size) {

    Debug_assert(size % FRAME_SIZE == 0);

    /* The number of needed pages */
    u32int pages = size / FRAME_SIZE;

    for(u32int i = 0; i < pages; i++) {

        heapTop -= FRAME_SIZE;
        Debug_assert(heapTop >= (char*) KERNEL_HEAP_VADDR)

        void* physicalAddress = VirtualMemory_getPhysicalAddress(heapTop);
        Debug_assert(physicalAddress != NULL);
        PhysicalMemory_freeFrame(physicalAddress);
        VirtualMemory_unmapPage(heapTop);

    }

}

PUBLIC Module* HeapMemory_getModule(void) {

    heapModule.moduleName = "Kernel Heap Manager";
    heapModule.init = &HeapMemory_init;
    heapModule.moduleID = MODULE_HEAP;
    heapModule.numberOfDependencies = 1;
    heapModule.dependencies[0] = MODULE_VMM;

    return &heapModule;
}