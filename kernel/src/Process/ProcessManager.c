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
#include <Process/Process.h>
#include <Lib/LinkedList.h>
#include <Drivers/Console.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module pmModule;
//PRIVATE LinkedList* processes;
PRIVATE Process* processes[2];
PRIVATE int currentProcessId;

/*=======================================================
    FUNCTION
=========================================================*/

#pragma GCC push_options
#pragma GCC optimize ("O0")
PRIVATE void test(void) {

    int i = 0;
    u16int* mem = (u16int*) 0xB8000;

    while(TRUE) {
        *mem = (i << 8) + 'A';
        i++;
    }
}
#pragma GCC pop_options

PRIVATE void ProcessManager_init(void) {

    //processes = LinkedList_new();
    //LinkedList_add(processes, initialProcess);
    Process* initialProcess = Process_new(0, "Test", NULL);
    currentProcessId = initialProcess->pid;

    Process* initialProcess2 = Process_new(1, "Test2", test);

    processes[0] = initialProcess;
    processes[1] = initialProcess2;


}

PRIVATE void setRegs(Regs* from, Regs* to) {

    to->cs = from->cs;
    to->eflags = from->eflags;
    to->eip = from->eip;
    to->eax = from->eax;
    to->ebx = from->ebx;
    to->edi = from->edi;
    to->esi = from->esi;
    to->ebp = from->ebp;
    to->esp = from->esp;
    to->ecx = from->ecx;
    to->edx = from->edx;
    to->ds = from->ds;
    to->es = from->es;
    to->fs = from->fs;
    to->intNo = from->intNo;
    to->errCode = from->intNo;
    to->esp0 = from->esp0;
    to->ss0 = from->ss0;

}

PUBLIC void ProcessManager_schedule(Regs* context) {

    setRegs(context, processes[currentProcessId]->kernelStackTop);

    switch(currentProcessId)
    {
    case 0:
      currentProcessId = 1;
      break;
    case 1:
      currentProcessId = 0;
      break;
    }

    setRegs(processes[currentProcessId]->kernelStackTop, context);
}

PUBLIC Module* ProcessManager_getModule(void) {

    pmModule.moduleName = "Process Manager";
    pmModule.init = &ProcessManager_init;
    pmModule.moduleID = MODULE_PROCESS;
    pmModule.numberOfDependencies = 1;
    pmModule.dependencies[0] = MODULE_HEAP;

    return &pmModule;
}