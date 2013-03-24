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
#include <Process/Scheduler.h>

/* Include Heap manager implementation */
/* #include <Memory/DumbHeapManager.h> */
#include <Memory/DougLea.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module heapModule;
PRIVATE char*  kernelHeapTop;

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

    kernelHeapTop = (void*) KERNEL_HEAP_BASE_VADDR;

    /* Point to heap manager implementation */
    HeapMemory_alloc   = &DougLea_malloc;
    HeapMemory_realloc = &DougLea_realloc;
    HeapMemory_calloc  = &DougLea_calloc;
    HeapMemory_free    = &DougLea_free;

}

PUBLIC void* HeapMemory_expand(ptrdiff_t size) {

    Debug_assert(size % FRAME_SIZE == 0); /* requested size needs to be page aligned */
    Debug_assert((u32int) kernelHeapTop % FRAME_SIZE == 0);  /* heap top needs to be page aligned */

    /* The number of needed pages */
    u32int pages = size / FRAME_SIZE;

    if(size >= 0) { /* Expand heap */

        Debug_assert((u32int) kernelHeapTop + size < KERNEL_HEAP_TOP_VADDR); /* heap should not overflow */
        void* ret = kernelHeapTop;

        for(u32int i = 0; i < pages; i++) {

            void* physicalAddress = PhysicalMemory_allocateFrame();

            if(physicalAddress == NULL) /* Are we out of physical memory? */
                Sys_panic("Out of physical memory!");

            if(Scheduler_getCurrentProcess == NULL || Scheduler_getCurrentProcess() == NULL)
                VirtualMemory_mapPage(VirtualMemory_getKernelDir(), kernelHeapTop, physicalAddress, MODE_KERNEL);
            else
                VirtualMemory_mapPage(Scheduler_getCurrentProcess()->pageDir, kernelHeapTop, physicalAddress, MODE_KERNEL);

            Memory_set(kernelHeapTop, 0, FRAME_SIZE); /* Nullify allocated frame */
            kernelHeapTop += FRAME_SIZE;

        }

        return ret;

    } else { /* Contract heap */

        Debug_assert((u32int) kernelHeapTop - size >= KERNEL_HEAP_BASE_VADDR); /* heap should not underflow */

        for(u32int i = 0; i < pages * -1; i++) {

            kernelHeapTop -= FRAME_SIZE;
            Debug_assert(kernelHeapTop >= (char*) KERNEL_HEAP_BASE_VADDR);

            void* physicalAddress = VirtualMemory_getPhysicalAddress(kernelHeapTop);
            Debug_assert(physicalAddress != NULL);
            PhysicalMemory_freeFrame(physicalAddress);

            if(Scheduler_getCurrentProcess == NULL || Scheduler_getCurrentProcess() == NULL)
                VirtualMemory_unmapPage(VirtualMemory_getKernelDir(), kernelHeapTop);
            else
                VirtualMemory_unmapPage(Scheduler_getCurrentProcess()->pageDir, kernelHeapTop);

        }

        return kernelHeapTop;

    }

}

PUBLIC void* HeapMemory_expandUser(ptrdiff_t size) {

    Process* currentProcess = Scheduler_getCurrentProcess();

    Debug_assert(size % FRAME_SIZE == 0); /* requested size needs to be page aligned */
    Debug_assert((u32int) currentProcess->userHeapTop % FRAME_SIZE == 0);  /* heap top needs to be page aligned */

    /* The number of needed pages */
    u32int pages = size / FRAME_SIZE;

    if(size >= 0) { /* Expand heap */

        Debug_assert((u32int) currentProcess->userHeapTop + size < USER_HEAP_TOP_VADDR); /* heap should not overflow */
        void* ret = currentProcess->userHeapTop;

        for(u32int i = 0; i < pages; i++) {

            void* physicalAddress = PhysicalMemory_allocateFrame();

            if(physicalAddress == NULL) /* Are we out of physical memory? */
                Sys_panic("Out of physical memory!");

            VirtualMemory_mapPage(Scheduler_getCurrentProcess()->pageDir, currentProcess->userHeapTop, physicalAddress, MODE_USER);

            Memory_set(currentProcess->userHeapTop, 0, FRAME_SIZE); /* Nullify allocated frame */
            currentProcess->userHeapTop += FRAME_SIZE;

        }

        return ret;

    } else { /* Contract heap */

        Debug_assert((u32int) currentProcess->userHeapTop - size >= USER_HEAP_BASE_VADDR); /* heap should not underflow */

        for(u32int i = 0; i < pages * -1; i++) {

            currentProcess->userHeapTop -= FRAME_SIZE;
            Debug_assert((char*) currentProcess->userHeapTop >= (char*) USER_HEAP_BASE_VADDR);

            void* physicalAddress = VirtualMemory_getPhysicalAddress(currentProcess->userHeapTop);
            Debug_assert(physicalAddress != NULL);
            PhysicalMemory_freeFrame(physicalAddress);
            VirtualMemory_unmapPage(Scheduler_getCurrentProcess()->pageDir, currentProcess->userHeapTop);

        }

        return currentProcess->userHeapTop;

    }

}

PUBLIC Module* HeapMemory_getModule(void) {

    if(!heapModule.isLoaded) {

        heapModule.moduleName = "Kernel Heap Manager";
        heapModule.init = &HeapMemory_init;
        heapModule.moduleID = MODULE_HEAP;
        heapModule.numberOfDependencies = 1;
        heapModule.dependencies[0] = MODULE_VMM;

    }

    return &heapModule;
}