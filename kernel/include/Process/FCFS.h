/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| FCFS.h (implements Scheduler)
|--------------------------------------------------------------------------
|
| DESCRIPTION:  First-Come, First-Served non-preemptive scheduler implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef FCFS_H
#define FCFS_H

#include <Process/ProcessManager.h>

/*-------------------------------------------------------------------------
| Add process
|--------------------------------------------------------------------------
| DESCRIPTION:     Adds a new process to scheduler's process list.
|
| PARAM:           'process'  the process to add
\------------------------------------------------------------------------*/
void FCFS_addProcess(Process* process);

/*-------------------------------------------------------------------------
| Remove process
|--------------------------------------------------------------------------
| DESCRIPTION:     Removes a process from scheduler's process list.
|
| PARAM:           'process'  the process to remove
\------------------------------------------------------------------------*/
void FCFS_removeProcess(Process* process);

/*-------------------------------------------------------------------------
| Get next process
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the next process to be switched to
|
| RETURN:          'Process*' the next process
\------------------------------------------------------------------------*/
Process* FCFS_getNextProcess(void);

/*-------------------------------------------------------------------------
| Get current process
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the current process.
|
| RETURN:          'Process*' the current process
\------------------------------------------------------------------------*/
Process* FCFS_getCurrentProcess(void);

#endif