/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| ProcessManager.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Process/ProcessManager.h>
#include <Process/Scheduler.h>
#include <Drivers/Console.h>
#include <Memory/VirtualMemory.h>
#include <Memory.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module pmModule;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void Test1() {

    while(TRUE)
        Console_printString("Task 1 says hi!\n");

}

PRIVATE void Test2() {

    while(TRUE)
        Console_printString("Task 2 says hi!\n");

}

PRIVATE void Test3() {

    while(TRUE)
        Console_printString("Task 3 says hi!\n");

}

PRIVATE void Test4() {

    while(TRUE)
        Console_printString("Task 4 says hi!\n");

}

PRIVATE void ProcessManager_init(void) {

    /* Create kernel process and add to scheduler */
    Process* initialProcess = Process_new(0, "Test", NULL, 0);
    Scheduler_addProcess(initialProcess);

    Process* test1 = Process_new(1, "Test1", Test1, 1);
    Scheduler_addProcess(test1);

    Process* test2 = Process_new(2, "Test2", Test2, 1);
    Scheduler_addProcess(test2);

    Process* test3 = Process_new(3, "Test3", Test3, 1);
    Scheduler_addProcess(test3);

    Process* test4 = Process_new(4, "Test4", Test4, 1);
    Scheduler_addProcess(test4);

}

PUBLIC void ProcessManager_switch(Regs* context) {

    /* Save process state */
    Process* currentProcess = Scheduler_getCurrentProcess();
    Debug_assert(currentProcess != NULL);
    currentProcess->kernelStack = context;

    /* Get next process from scheduler */
    Process* next = Scheduler_getNextProcess();
    Debug_assert(next != NULL);

    if(currentProcess == next) /* No need for a context switch */
        return;

    /* Do context switch */
    VirtualMemory_switchPageDir(next->pageDir); /* Switch to new process' address space */
    Debug_assert(next->kernelStack != NULL);
    asm volatile("mov %0, %%DR0" : : "r" (next->kernelStack)); /* Store new process ESP in DR0 register */

}

PUBLIC Module* ProcessManager_getModule(void) {

    pmModule.moduleName = "Process Manager";
    pmModule.init = &ProcessManager_init;
    pmModule.moduleID = MODULE_PROCESS;
    pmModule.numberOfDependencies = 1;
    pmModule.dependencies[0] = MODULE_SCHEDULER;

    return &pmModule;
}