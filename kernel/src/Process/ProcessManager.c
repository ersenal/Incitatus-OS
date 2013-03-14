/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| ProcessManager.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Handles process creation, destruction, switch.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Process/ProcessManager.h>
#include <Process/Scheduler.h>
#include <Drivers/Console.h>
#include <Memory/VirtualMemory.h>
#include <Memory/HeapMemory.h>
#include <Memory/PhysicalMemory.h>
#include <Memory.h>
#include <Lib/String.h>
#include <X86/GDT.h>
#include <Process/Mutex.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE u32int pid = 1;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE Process* ProcessManager_newProcess(void* entry, u32int binarySize, bool mode) {

    Process* self = HeapMemory_calloc(1, sizeof(Process));
    Debug_assert(self != NULL);

    self->pid = pid;
    self->binaryEntry = entry;
    self->binarySize = binarySize;

    if(mode == USER_PROCESS) { /* User process */

        Regs registers;
        Memory_set(&registers, 0, sizeof(Regs));

        //TODO: fix heap
        // self->userHeapBase = (void*) USER_HEAP_BASE_VADDR;

        registers.eflags = 0x202; /* Interrupt enable flag */
        registers.eip    = USER_CODE_BASE_VADDR; /* Initial code entry point */
        registers.intNo  = IRQ0;

        /* Add 3 so that they have an RPL of 3 (User ring) */
        registers.cs     = USER_CODE_SEGMENT | 3;
        registers.ds     = USER_DATA_SEGMENT | 3;
        registers.es     = USER_DATA_SEGMENT | 3;
        registers.fs     = USER_DATA_SEGMENT | 3;
        registers.gs     = USER_DATA_SEGMENT | 3;

        /* Create a new page directory for process */
        VirtualMemory_createPageDirectory(self);

        /* Map kernel bottom 4MB + kernel heap */
        VirtualMemory_mapKernel(self);

        /* Allocate kernel stack - 4KB */
        u32int* stack = HeapMemory_calloc(1, FRAME_SIZE);
        Debug_assert(stack != NULL);
        self->kernelStackBase = stack;

        /* Allocate and map user stack - Currently 4KB */
        Debug_assert(USER_STACK_SIZE == FRAME_SIZE);
        self->userStackBase = (void*) USER_STACK_BASE_VADDR;
        char* uStack = PhysicalMemory_allocateFrame();
        VirtualMemory_switchPageDir(self->pageDir);
        VirtualMemory_mapPage((void*) USER_STACK_BASE_VADDR, uStack, MODE_USER);
        self->userStack = (void*) ((char*) self->userStackBase + FRAME_SIZE - sizeof(Regs));

        /* Set up initial user ss and esp */
        registers.esp0   = (u32int) self->userStack;;
        registers.ss0    = USER_DATA_SEGMENT | 3;;
        Memory_copy(self->userStack, &registers, sizeof(Regs));

        /* Setup kernel stack */
        stack = (u32int*) ((char*) stack + FRAME_SIZE - sizeof(Regs));
        Memory_copy(stack, &registers, sizeof(Regs));
        self->kernelStack = stack;

        self->fileNodes = ArrayList_new(1);

    } else { /* Kernel process */

        self->pageDir = VirtualMemory_getKernelDir();

    }

    pid++;
    self->status = PROCESS_CREATED;

    return self;

}

PRIVATE void ProcessManager_destroyProcess(Process* process) {

    HeapMemory_free(process->kernelStackBase);

    if(process->fileNodes != NULL) {

        /* close all opened files */
        for(u32int i = 0; i < ArrayList_getSize(process->fileNodes); i++) {

            VFSNode* file = ArrayList_get(process->fileNodes, i);
            VFS_closeFile(file);

        }

        ArrayList_destroy(process->fileNodes);

    }

    HeapMemory_free(process);
    VirtualMemory_destroyPageDirectory(process);

}

PUBLIC void ProcessManager_switch(Regs* context) {

    /* Save process state */
    Process* currentProcess = Scheduler_getCurrentProcess();
    Debug_assert(currentProcess != NULL);
    currentProcess->userStack = context;
    currentProcess->status = PROCESS_WAITING;

    /* Get next process from scheduler */
    Process* next = Scheduler_getNextProcess();
    Debug_assert(next != NULL);
    next->status = PROCESS_RUNNING;
    if(currentProcess == next) /* No need for a context switch */
        return;

    /* Do context switch */
    Debug_assert(next->kernelStack != NULL);
    GDT_setTSS(KERNEL_DATA_SEGMENT, (u32int) next->kernelStack);

    VirtualMemory_switchPageDir(next->pageDir); /* Switch to new process' address space */
    asm volatile("mov %0, %%DR0" : : "r" (next->userStack)); /* Store new process ESP in DR0 register */

}

PUBLIC void ProcessManager_killProcess(int exitCode) {

    Process* current = Scheduler_getCurrentProcess();
    Debug_logInfo("%s%d%c%s%s%d", "PID:", current->pid, ' ', current->name, " exited with code ", exitCode);
    Scheduler_removeProcess(current);
    ProcessManager_destroyProcess(current);

    /* Get next process from scheduler */
    Process* next = Scheduler_getNextProcess();
    Debug_assert(next != NULL);
    Debug_assert(next != current);
    next->status = PROCESS_RUNNING;

    /* Do context switch */
    Debug_assert(next->kernelStack != NULL);
    GDT_setTSS(KERNEL_DATA_SEGMENT, (u32int) next->kernelStack);

    VirtualMemory_switchPageDir(next->pageDir); /* Switch to new process' address space */
    asm volatile("mov %0, %%DR0" : : "r" (next->userStack)); /* Store new process ESP in DR0 register */

    //TODO: Fix this hack
    asm volatile("mov %0, %%DR1" : : "r" (0xDEADBEEF)); /* Store 1 at DR1 in order to distinguish the procedure */

}

PUBLIC Process* ProcessManager_spawnProcess(const char* binary) {

    VFSNode* bin = VFS_openFile(binary, "r");
    Debug_assert(bin != NULL);

    char* buffer = HeapMemory_calloc(1, bin->fileSize + 1);
    Debug_assert(buffer != NULL);
    bin->vfs->read(bin, 0, bin->fileSize, buffer);

    Process* p = ProcessManager_newProcess((void*) buffer, bin->fileSize, USER_PROCESS);
    p->workingDirectory = VFS_getParent(bin);
    String_copy(p->name, bin->fileName); /* Set process name */
    Debug_assert(p != NULL);

    /* Copy user code from kernel heap to user space */
    void* f = PhysicalMemory_allocateFrame(); //TODO: what if user code > 4kB
    VirtualMemory_mapPage((void*) USER_CODE_BASE_VADDR, f, MODE_USER);
    Memory_copy((void*) USER_CODE_BASE_VADDR, buffer, bin->fileSize);

    /* Halfway through newProcess(), we switch to new process' directory. Now return back to original directory */
    if(Scheduler_getCurrentProcess() != NULL)
        VirtualMemory_switchPageDir(Scheduler_getCurrentProcess()->pageDir);

    HeapMemory_free(buffer);
    VFS_closeFile(bin);
    Scheduler_addProcess(p);
    return p;

}