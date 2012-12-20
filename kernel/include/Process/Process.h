/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Process.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef PROCESS_H
#define PROCESS_H

#include <Common.h>
#include <X86/IDT.h>

/*=======================================================
    STRUCT
=========================================================*/
typedef struct Process Process;

struct Process {

    u32int pid;
    char   name[32];
    void*  userHeapBase;
    void*  kernelStack;
    void*  kernelStackBase;
    void*  pageDir;

};

/*=======================================================
    FUNCTION
=========================================================*/
Process* Process_new(u32int id, char* name, void* entry, bool mode);
void Process_printRegisters(Process* process);
void Process_destroy(Process* self);

#endif