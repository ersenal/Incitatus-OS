/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| ProcessManager.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Handles process creation, destruction, switch.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef PROCESS_M_H
#define PROCESS_M_H

#include <Module.h>
#include <X86/IDT.h>
#include <FileSystem/VFS.h>
#include <Lib/ArrayList.h>

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
    void*  userHeapTop;
    void*  kernelStack;
    void*  kernelStackBase;
    void*  userStack;
    void*  userStackBase;
    void*  pageDir;

    VFSNode* workingDirectory;
    ArrayList* fileNodes;

};

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Process switch
|--------------------------------------------------------------------------
| DESCRIPTION:     Performs a process switch.
|                  Called by PIT(See PIT.c : PIT8253_timerHandler)
|
| PARAM:          'Regs*' the context
\------------------------------------------------------------------------*/
void  ProcessManager_switch(Regs* context);

/*-------------------------------------------------------------------------
| Kill process
|--------------------------------------------------------------------------
| DESCRIPTION:     Terminates the current process and switches to next one.
|
| PARAM:           'exitCode' the exit code
\------------------------------------------------------------------------*/
void ProcessManager_killProcess(int exitCode);

/*-------------------------------------------------------------------------
| Spawn process
|--------------------------------------------------------------------------
| DESCRIPTION:     Spawns a new process from an executable binary file.
|
| PARAM:           'binary' the binary pathname
|
| RETURN:          'Process*' the process that was spawned
\------------------------------------------------------------------------*/
Process* ProcessManager_spawnProcess(const char* binary);

/*-------------------------------------------------------------------------
| Wait process ID
|--------------------------------------------------------------------------
| DESCRIPTION:    Causes the current process to wait for specified process' termination.
|
| PARAM:          'Process*' the process to wait for
\------------------------------------------------------------------------*/
void ProcessManager_waitPID(Process* process);

/*-------------------------------------------------------------------------
| Block current process
|--------------------------------------------------------------------------
| DESCRIPTION:     Blocks the current process from further execution.
|
\------------------------------------------------------------------------*/
void ProcessManager_blockCurrentProcess(void);

/*-------------------------------------------------------------------------
| Get process management module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the process management module.
|
\------------------------------------------------------------------------*/
Module* ProcessManager_getModule(void);

#endif