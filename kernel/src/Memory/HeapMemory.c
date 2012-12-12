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

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module heapModule;
PRIVATE char*  heapTop;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void HeapMemory_init(void) {

    heapTop = (void*) KERNEL_HEAP_VADDR;

}

PUBLIC void* HeapMemory_expand(u32int size) {

    Debug_assert(size % FRAME_SIZE == 0);

    /* The number of needed pages */
    u32int pages = size / FRAME_SIZE;

    void* ret = heapTop;

    for(u32int i = 0; i < pages; i++) {

        void* physicalAddress = PhysicalMemory_allocateFrame();
        Debug_assert(physicalAddress != NULL);
        VirtualMemory_mapPage(heapTop, physicalAddress);
        Memory_set(heapTop, 0, FRAME_SIZE);
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