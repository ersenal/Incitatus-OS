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
    Process* initialProcess = Process_new(0, "Test", NULL);
    Scheduler_addProcess(initialProcess);

    Process* test1 = Process_new(1, "Test", Test1);
    Scheduler_addProcess(test1);

    Process* test2 = Process_new(2, "Test", Test2);
    Scheduler_addProcess(test2);

    Process* test3 = Process_new(3, "Test", Test3);
    Scheduler_addProcess(test3);

    Process* test4 = Process_new(4, "Test", Test4);
    Scheduler_addProcess(test4);

}

PUBLIC void ProcessManager_switch(Regs* context) {

    /* Save process state */
    Process* currentProcess = Scheduler_getCurrentProcess();
    Memory_copy(currentProcess->kernelStackTop, context, sizeof(Regs));

    /* Get next process from scheduler and do context switch */
    Process* next = Scheduler_getNextProcess();
    Memory_copy(context, next->kernelStackTop, sizeof(Regs));

}

PUBLIC Module* ProcessManager_getModule(void) {

    pmModule.moduleName = "Process Manager";
    pmModule.init = &ProcessManager_init;
    pmModule.moduleID = MODULE_PROCESS;
    pmModule.numberOfDependencies = 1;
    pmModule.dependencies[0] = MODULE_SCHEDULER;

    return &pmModule;
}