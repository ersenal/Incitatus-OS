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

//TODO: add comment
void FCFS_addProcess(Process* process);
void FCFS_removeProcess(Process* process);
Process* FCFS_getNextProcess(void);
Process* FCFS_getCurrentProcess(void);

#endif