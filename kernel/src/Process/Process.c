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

    Process* self = HeapMemory_calloc(1, sizeof(Process));
    Memory_copy(self->name, name, 32);
    self->pid = id;

    if(mode == 1) { /* User process */

        self->userHeapBase = (void*) USER_HEAP_BASE_VADDR;

        /* Create a new page directory for process */
        VirtualMemory_createPageDirectory(self);

        /* Map kernel bottom 4MB + kernel heap */
        VirtualMemory_mapKernel(self);

        /* Allocate and map user stack */
        Debug_assert(USER_STACK_SIZE % FRAME_SIZE == 0);
        for(int i = 0; i < USER_STACK_SIZE / FRAME_SIZE; i++) {

            void* frame = PhysicalMemory_allocateFrame();
            Debug_assert(frame != NULL);
            VirtualMemory_mapPage((void*) (USER_STACK_BASE_VADDR + (i * FRAME_SIZE)), frame);

        }

        self->registers.eflags = 0x200; /* Re-enable interrupts in user space */
        self->registers.eip    = (u32int) entry; /* Initial code entry point */
        self->registers.intNo  = IRQ0;
        self->registers.cs     = KERNEL_CODE_SEGMENT;
        self->registers.ds     = KERNEL_DATA_SEGMENT;
        self->registers.es     = KERNEL_DATA_SEGMENT;
        self->registers.fs     = KERNEL_DATA_SEGMENT;
        self->registers.gs     = KERNEL_DATA_SEGMENT;

        /* Set up initial user ss and esp */
        self->registers.esp0   = 0;
        self->registers.ss0    = 0;

    } else { /* Kernel process */

        self->pageDir = VirtualMemory_getKernelDir();

        self->registers.eflags = 0x200; /* Re-enable interrupts */
        self->registers.intNo  = IRQ0;
        self->registers.cs     = KERNEL_CODE_SEGMENT;
        self->registers.ds     = KERNEL_DATA_SEGMENT;
        self->registers.es     = KERNEL_DATA_SEGMENT;
        self->registers.fs     = KERNEL_DATA_SEGMENT;
        self->registers.gs     = KERNEL_DATA_SEGMENT;

    }

    return self;

}

PUBLIC void Process_printRegisters(Process* process) {

    Console_printf("%s%d%c", "CS: ", process->registers.cs, '\n');
    Console_printf("%s%d%c", "EIP: ", process->registers.eip, '\n');
    Console_printf("%s%d%c", "GS: ", process->registers.gs, '\n');
    Console_printf("%s%d%c", "FS: ", process->registers.fs, '\n');
    Console_printf("%s%d%c", "ES: ", process->registers.es, '\n');
    Console_printf("%s%d%c", "DS: ", process->registers.ds, '\n');
    Console_printf("%s%d%c", "EDI: ", process->registers.edi, '\n');
    Console_printf("%s%d%c", "ESI: ", process->registers.esi, '\n');
    Console_printf("%s%d%c", "EBP: ", process->registers.ebp, '\n');
    Console_printf("%s%d%c", "ESP: ", process->registers.esp, '\n');
    Console_printf("%s%d%c", "EBX: ", process->registers.ebx, '\n');
    Console_printf("%s%d%c", "EDX: ", process->registers.edx, '\n');
    Console_printf("%s%d%c", "ECS: ", process->registers.ecx, '\n');
    Console_printf("%s%d%c", "EAX: ", process->registers.eax, '\n');
    Console_printf("%s%d%c", "EFLAGS: ", process->registers.eflags, '\n');
    Console_printf("%s%d%c", "SS0: ", process->registers.ss0, '\n');
    Console_printf("%s%d%c", "ESP0: ", process->registers.esp0, '\n');
    Console_printf("%s%d%c", "IntNo: ", process->registers.intNo, '\n');
    Console_printf("%s%d%c", "ErrCode: ", process->registers.errCode, '\n');

}

PUBLIC void Process_destroy(Process* self) {

    VirtualMemory_destroyPageDirectory(self);
    HeapMemory_free(self);

}