/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| FCFS.c (implements Scheduler)
|--------------------------------------------------------------------------
|
| DESCRIPTION:  First-Come, First-Served non-preemptive scheduler implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Process/FCFS.h>
#include <Lib/LinkedList.h>
#include <Memory/HeapMemory.h>
#include <Debug.h>
#include <Common.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE LinkedList* processes;
PRIVATE Process* currentProcess;

/*=======================================================
    FUNCTION
=========================================================*/

PUBLIC void FCFS_addProcess(Process* process) {

    Debug_assert(process != NULL);

    if(processes == NULL) { /* FCFS initialisation */

        processes = LinkedList_new();
        currentProcess = process;

    }

    process->status = PROCESS_WAITING;
    LinkedList_add(processes, process);

}

PUBLIC void FCFS_removeProcess(Process* process) {

    Debug_assert(process != NULL && processes != NULL);
    Debug_assert(process->pid != KERNEL_PID); /* Can't kill kernel process */

    LinkedList_remove(processes, process);
    process->status = PROCESS_TERMINATED;

}

PUBLIC Process* FCFS_getNextProcess(void) {

    currentProcess = LinkedList_getFront(processes);
    return currentProcess;

}

PUBLIC Process* FCFS_getCurrentProcess(void) {

    return currentProcess;

}
