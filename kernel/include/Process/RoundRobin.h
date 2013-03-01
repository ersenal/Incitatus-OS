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

#include <Process/Process.h>

void RoundRobin_init(Process* firstProcess);
void RoundRobin_addProcess(Process* process);
void RoundRobin_removeProcess(Process* process);
Process* RoundRobin_getNextProcess(void);
Process* RoundRobin_getCurrentProcess(void);

#endif