/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Scheduler.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Process scheduler interface.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Process/ProcessManager.h>

/*=======================================================
    INTERFACE
=========================================================*/
extern void (*Scheduler_addProcess) (Process* process);
extern void (*Scheduler_removeProcess) (Process* process);
extern Process* (*Scheduler_getNextProcess) (void);
extern Process* (*Scheduler_getCurrentProcess) (void);

/*=======================================================
    FUNCTION
=========================================================*/
Module* Scheduler_getModule(void);

#endif