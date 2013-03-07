/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Process.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Handles process creation, destruction.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Process/Process.h>
#include <Memory/HeapMemory.h>
#include <Memory/VirtualMemory.h>
#include <Memory/PhysicalMemory.h>
#include <X86/GDT.h>
#include <Debug.h>
#include <Memory.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE u32int pid = 0;

/*=======================================================
    FUNCTION
=========================================================*/

PUBLIC Process* Process_new(void* entry, bool mode) {

    Process* self = HeapMemory_calloc(1, sizeof(Process));
    Debug_assert(self != NULL);
    self->pid = pid;

    if(mode == USER_PROCESS) { /* User process */

        Regs registers;
        Memory_set(&registers, 0, sizeof(Regs));

        self->userHeapBase = (void*) USER_HEAP_BASE_VADDR;

        registers.eflags = 0x202; /* Interrupt enable flag */
        registers.eip    = (u32int) entry; /* Initial code entry point */
        registers.intNo  = IRQ0;

        /* Add 3 so that they have an RPL of 3 (User ring) */
        registers.cs     = USER_CODE_SEGMENT | 3;
        registers.ds     = USER_DATA_SEGMENT | 3;
        registers.es     = USER_DATA_SEGMENT | 3;
        registers.fs     = USER_DATA_SEGMENT | 3;
        registers.gs     = USER_DATA_SEGMENT | 3;

        /* Allocate kernel stack - 4KB */
        u32int* stack = HeapMemory_calloc(1, FRAME_SIZE);
        Debug_assert(stack != NULL);
        self->kernelStackBase = stack;

        /* Allocate and map user stack - Currently 4KB */
        Debug_assert(USER_STACK_SIZE == FRAME_SIZE);
        u32int* ustack = HeapMemory_calloc(1, FRAME_SIZE);
        Debug_assert(ustack != NULL);
        self->userStack = (void*) ((char*) ustack + FRAME_SIZE - sizeof(Regs));

        /* Set up initial user ss and esp */
        registers.esp0   = (u32int) self->userStack;;
        registers.ss0    = USER_DATA_SEGMENT | 3;;
        Memory_copy(self->userStack, &registers, sizeof(Regs));

        /* Setup kernel stack */
        stack = (u32int*) ((char*) stack + FRAME_SIZE - sizeof(Regs));
        Memory_copy(stack, &registers, sizeof(Regs));
        self->kernelStack = stack;

        /* Create a new page directory for process */
        VirtualMemory_createPageDirectory(self);

        /* Map kernel bottom 4MB + kernel heap */
        VirtualMemory_mapKernel(self);

    } else { /* Kernel process */

        self->pageDir = VirtualMemory_getKernelDir();

    }

    pid++;
    self->status = PROCESS_CREATED;
    return self;

}

PUBLIC void Process_destroy(Process* self) {

    VirtualMemory_destroyPageDirectory(self);
    HeapMemory_free(self->kernelStackBase);
    HeapMemory_free(self);

}