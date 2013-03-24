/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| RoundRobin.c (implements Scheduler)
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Round robin process scheduler implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Process/RoundRobin.h>
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

PUBLIC void RoundRobin_addProcess(Process* process) {

    Debug_assert(process != NULL);
    Debug_assert(process->status = PROCESS_CREATED);

    if(processes == NULL) { /* RoundRobin initialisation */

        processes = LinkedList_new();
        currentProcess = process;

    }

    process->status = PROCESS_WAITING;
    LinkedList_add(processes, process);

}

PUBLIC void RoundRobin_removeProcess(Process* process) {

    Debug_assert(process != NULL && processes != NULL);
    Debug_assert(process->pid != KERNEL_PID); /* Can't remove kernel process */

    process->status = PROCESS_TERMINATED;
    LinkedList_remove(processes, process);

}

PUBLIC Process* RoundRobin_getNextProcess(void) {

    /* Remove process from head of queue and add as the last element */
    Process* p = LinkedList_removeFromFront(processes);

    LinkedList_add(processes, p);
    currentProcess = p;
    return p;

}

PUBLIC Process* RoundRobin_getCurrentProcess(void) {

    return currentProcess;

}
