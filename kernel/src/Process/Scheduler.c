/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Scheduler.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Process scheduler interface.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Process/Scheduler.h>
#include <Debug.h>

/* Include scheduler implementation */
/* #include <Process/FCFS.h> */
#include <Process/RoundRobin.h>


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

PUBLIC void Scheduler_init(void) {

    Debug_logInfo("%s", "Initialising Scheduler");

    /* Point to scheduler implementation */
    Scheduler_addProcess        = &RoundRobin_addProcess;
    Scheduler_removeProcess     = &RoundRobin_removeProcess;
    Scheduler_getNextProcess    = &RoundRobin_getNextProcess;
    Scheduler_getCurrentProcess = &RoundRobin_getCurrentProcess;

}