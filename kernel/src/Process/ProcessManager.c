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
#include <Drivers/VGA.h>
#include <Process/Mutex.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module pmModule;
PRIVATE u32int pid = 0;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void Test2() {

    u32int i = 0;

    while(1) {

        VGA_put(1, i);
        i++;

    }

}

PRIVATE void Test3() {

    u32int i = 0;

    while(1) {

        VGA_put(0, i);
        i++;

    }

}

PRIVATE void ProcessManager_init(void) {

    Process* test1 = ProcessManager_newProcess(NULL, USER_PROCESS);
    Scheduler_addProcess(test1);
    String_copy(test1->name, "Test1");

    GDT_setTSS(KERNEL_DATA_SEGMENT, (u32int) test1->kernelStack);

    Process* test2 = ProcessManager_newProcess(Test2, USER_PROCESS);
    String_copy(test2->name, "Test2");
    Scheduler_addProcess(test2);

    Process* test3 = ProcessManager_newProcess(Test3, USER_PROCESS);
    String_copy(test3->name, "Test3");
    Scheduler_addProcess(test3);

}

PRIVATE void ProcessManager_destroyProcess(Process* process) {

    HeapMemory_free(process->kernelStackBase);
    HeapMemory_free(process->userStackBase);

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

PUBLIC Process* ProcessManager_newProcess(void* entry, bool mode) {

    Process* self = HeapMemory_calloc(1, sizeof(Process));
    Debug_assert(self != NULL);

    self->pid = pid;
    self->workingDirectory = NULL; //TODO: implement

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
        self->userStackBase = HeapMemory_calloc(1, FRAME_SIZE);
        Debug_assert(self->userStackBase != NULL);
        self->userStack = (void*) ((char*) self->userStackBase + FRAME_SIZE - sizeof(Regs));

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

        self->fileNodes = ArrayList_new(1);

    } else { /* Kernel process */

        self->pageDir = VirtualMemory_getKernelDir();

    }

    pid++;
    self->status = PROCESS_CREATED;
    return self;

}

PUBLIC void ProcessManager_killProcess(int exitCode) {

    Debug_assert(exitCode == 0); /* Normal termination */
    Process* current = Scheduler_getCurrentProcess();
    current->status = PROCESS_TERMINATED;
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
    asm volatile("mov %0, %%DR1" : : "r" (0xDEADBEEF)); /* Store 1 at DR1 in order to distinguish the procedure */

}

PUBLIC Module* ProcessManager_getModule(void) {

    if(!pmModule.isLoaded) {

        pmModule.moduleName = "Process Manager";
        pmModule.init = &ProcessManager_init;
        pmModule.moduleID = MODULE_PROCESS;
        pmModule.numberOfDependencies = 1;
        pmModule.dependencies[0] = MODULE_SCHEDULER;

    }

    return &pmModule;
}