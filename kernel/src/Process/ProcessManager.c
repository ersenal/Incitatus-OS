/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| ProcessManager.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Handles context switch.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Process/ProcessManager.h>
#include <Process/Scheduler.h>
#include <Drivers/Console.h>
#include <Memory/VirtualMemory.h>
#include <Memory.h>
#include <Lib/String.h>
#include <X86/GDT.h>
#include <Drivers/VGA.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module pmModule;

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

PRIVATE void ProcessManager_init(void) {

    Process* test1 = Process_new(NULL, USER_PROCESS);
    Scheduler_addProcess(test1);
    String_copy(test1->name, "Test1");

    GDT_setTSS(KERNEL_DATA_SEGMENT, (u32int) test1->kernelStack);

    Process* test2 = Process_new(Test2, USER_PROCESS);
    String_copy(test2->name, "Test2");
    Scheduler_addProcess(test2);

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
    asm volatile("mov %0, %%DR1" : : "r" (next)); /* Store process pointer in DR1 register */

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