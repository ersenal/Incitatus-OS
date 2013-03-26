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

/*-------------------------------------------------------------------------
| Add process
|--------------------------------------------------------------------------
| DESCRIPTION:     Adds a new process to scheduler's process list.
|
| PARAM:           'process'  the process to add
\------------------------------------------------------------------------*/
extern void (*Scheduler_addProcess) (Process* process);

/*-------------------------------------------------------------------------
| Remove process
|--------------------------------------------------------------------------
| DESCRIPTION:     Removes a process from scheduler's process list.
|
| PARAM:           'process'  the process to remove
\------------------------------------------------------------------------*/
extern void (*Scheduler_removeProcess) (Process* process);

/*-------------------------------------------------------------------------
| Get next process
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the next process to be switched to
|
| RETURN:          'Process*' the next process
\------------------------------------------------------------------------*/
extern Process* (*Scheduler_getNextProcess) (void);

/*-------------------------------------------------------------------------
| Get current process
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the current process.
|
| RETURN:          'Process*' the current process
\------------------------------------------------------------------------*/
extern Process* (*Scheduler_getCurrentProcess) (void);

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Initialise Scheduler
|--------------------------------------------------------------------------
| DESCRIPTION:     Initialises the Scheduler.
|
\------------------------------------------------------------------------*/
void Scheduler_init(void);

#endif