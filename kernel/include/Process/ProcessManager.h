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

//TODO: comments
void  ProcessManager_switch(Regs* context);
void ProcessManager_killProcess(int exitCode);
Process* ProcessManager_spawnProcess(const char* binary);
void ProcessManager_waitPID(Process* process);
void ProcessManager_blockCurrentProcess(void);
Module* ProcessManager_getModule(void);

#endif