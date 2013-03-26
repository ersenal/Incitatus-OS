/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| RoundRobin.h (implements Scheduler)
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Round robin process scheduler implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include <Process/ProcessManager.h>

/*-------------------------------------------------------------------------
| Add process
|--------------------------------------------------------------------------
| DESCRIPTION:     Adds a new process to scheduler's process list.
|
| PARAM:           'process'  the process to add
\------------------------------------------------------------------------*/
void RoundRobin_addProcess(Process* process);

/*-------------------------------------------------------------------------
| Remove process
|--------------------------------------------------------------------------
| DESCRIPTION:     Removes a process from scheduler's process list.
|
| PARAM:           'process'  the process to remove
\------------------------------------------------------------------------*/
void RoundRobin_removeProcess(Process* process);

/*-------------------------------------------------------------------------
| Get next process
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the next process to be switched to
|
| RETURN:          'Process*' the next process
\------------------------------------------------------------------------*/
Process* RoundRobin_getNextProcess(void);

/*-------------------------------------------------------------------------
| Get current process
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the current process.
|
| RETURN:          'Process*' the current process
\------------------------------------------------------------------------*/
Process* RoundRobin_getCurrentProcess(void);

#endif