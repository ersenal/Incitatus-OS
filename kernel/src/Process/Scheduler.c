/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Scheduler.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Process/Scheduler.h>

/* Include scheduler implementation */
#include <Process/RoundRobin.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module schedulerModule;

/*=======================================================
    PUBLIC DATA
=========================================================*/
PUBLIC void (*Scheduler_addProcess) (Process* process);
PUBLIC void (*Scheduler_removeProcess) (Process* process);
PUBLIC Process* (*Scheduler_getNextProcess) (void);
PUBLIC Process* (*Scheduler_getCurrentProcess) (void);

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void Scheduler_init(void) {

    /* Point to scheduler implementation */
    Scheduler_addProcess        = &RoundRobin_addProcess;
    Scheduler_removeProcess     = &RoundRobin_removeProcess;
    Scheduler_getNextProcess    = &RoundRobin_getNextProcess;
    Scheduler_getCurrentProcess = &RoundRobin_getCurrentProcess;

}

PUBLIC Module* Scheduler_getModule(void) {

    schedulerModule.moduleName = "Process Scheduler";
    schedulerModule.moduleID = MODULE_SCHEDULER;
    schedulerModule.init = &Scheduler_init;
    schedulerModule.numberOfDependencies = 1;
    schedulerModule.dependencies[0] = MODULE_HEAP;

    return &schedulerModule;

}