/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| RoundRobin.c (implements Scheduler)
|--------------------------------------------------------------------------
|
| DESCRIPTION:
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

    if(processes == NULL) { /* RoundRobin initialisation */

        processes = LinkedList_new();
        currentProcess = process;

    }

    process->status = PROCESS_WAITING;
    LinkedList_push(processes, process);

}

PUBLIC void RoundRobin_removeProcess(Process* process) {

    Debug_assert(process != NULL && processes != NULL);

    LinkedList_remove(processes, process);
    process->status = PROCESS_TERMINATED;
    Process_destroy(process);

}

PUBLIC Process* RoundRobin_getNextProcess(void) {

    /* Remove process from head of queue and add as the last element */
    Process* p = LinkedList_removeFromFront(processes);

    Debug_assert(p->status == PROCESS_WAITING);

    LinkedList_push(processes, p);
    currentProcess = p;
    return p;

}

PUBLIC Process* RoundRobin_getCurrentProcess(void) {

    return currentProcess;

}
