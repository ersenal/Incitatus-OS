/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Process.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:
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

PUBLIC Process* Process_new(u32int id, char* name, void* entry, bool mode) {

    UNUSED(name);

    Process* self = HeapMemory_calloc(1, sizeof(Process));
    Debug_assert(self != NULL);
    self->pid = id;

    Regs registers;
    Memory_set(&registers, 0, sizeof(Regs));

    if(mode == USER_PROCESS) { /* User process */

        self->userHeapBase = (void*) USER_HEAP_BASE_VADDR;

        registers.eflags = 0x200; /* Interrupt enable flag */
        registers.eip    = (u32int) entry; /* Initial code entry point */
        registers.intNo  = IRQ0;
        registers.cs     = KERNEL_CODE_SEGMENT;
        registers.ds     = KERNEL_DATA_SEGMENT;
        registers.es     = KERNEL_DATA_SEGMENT;
        registers.fs     = KERNEL_DATA_SEGMENT;
        registers.gs     = KERNEL_DATA_SEGMENT;

        /* Set up initial user ss and esp */
        registers.esp0   = 0;
        registers.ss0    = 0;

        /* Allocate kernel stack - 4KB */
        u32int* stack = HeapMemory_calloc(1, FRAME_SIZE);
        Debug_assert(stack != NULL);
        self->kernelStackBase = stack;

        stack = (u32int*) ((char*) stack + FRAME_SIZE - sizeof(Regs));
        Memory_copy(stack, &registers, sizeof(Regs));
        self->kernelStack = stack;

        /* Create a new page directory for process */
        VirtualMemory_createPageDirectory(self);

        /* Map kernel bottom 4MB + kernel heap */
        VirtualMemory_mapKernel(self);

        /* Allocate and map user stack - Currently 4KB */
        Debug_assert(USER_STACK_SIZE % FRAME_SIZE == 0);
        for(int i = 0; i < USER_STACK_SIZE / FRAME_SIZE; i++) {

            void* frame = PhysicalMemory_allocateFrame();
            Debug_assert(frame != NULL);
            VirtualMemory_mapPage((void*) (USER_STACK_BASE_VADDR + (i * FRAME_SIZE)), frame);

        }

    } else { /* Kernel process */

        self->pageDir = VirtualMemory_getKernelDir();

        registers.eflags = 0x200; /* Interrupt enable flag */
        registers.intNo  = IRQ0;
        registers.cs     = KERNEL_CODE_SEGMENT;
        registers.ds     = KERNEL_DATA_SEGMENT;
        registers.es     = KERNEL_DATA_SEGMENT;
        registers.fs     = KERNEL_DATA_SEGMENT;
        registers.gs     = KERNEL_DATA_SEGMENT;

    }

    self->status = PROCESS_CREATED;
    return self;

}

PUBLIC void Process_destroy(Process* self) {

    VirtualMemory_destroyPageDirectory(self);
    HeapMemory_free(self->kernelStackBase);
    HeapMemory_free(self);

}