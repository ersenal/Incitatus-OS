/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Process.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Handles process creation, destruction.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef PROCESS_H
#define PROCESS_H

#include <Common.h>
#include <X86/IDT.h>

/*=======================================================
    DEFINE
=========================================================*/

/* Kernel process id */
#define KERNEL_PID 0

/* Process Type */
#define KERNEL_PROCESS 0
#define USER_PROCESS   1

/* Process State */
#define PROCESS_CREATED     1
#define PROCESS_WAITING     2
#define PROCESS_RUNNING     3
#define PROCESS_BLOCKED     4
#define PROCESS_TERMINATED  5

/*=======================================================
    STRUCT
=========================================================*/
typedef struct Process Process;

struct Process {

    u32int pid;
    u8int  status;
    char   name[64];
    void*  userHeapBase;
    void*  kernelStack;
    void*  kernelStackBase;
    void*  userStack;
    void*  userStackBase;
    void*  pageDir;

};

/*=======================================================
    FUNCTION
=========================================================*/
Process* Process_new(void* entry, bool mode);
void Process_destroy(Process* self);

#endif