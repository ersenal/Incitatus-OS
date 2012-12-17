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
#include <Memory/VirtualMemory.h>
#include <X86/IDT.h>

/*=======================================================
    STRUCT
=========================================================*/
typedef struct Process Process;

struct Process {

    u32int pid;
    Regs*  kernelStackTop;
    char*  name;
    void*  entry;

};

/*=======================================================
    FUNCTION
=========================================================*/
Process* Process_new(u32int id, char* name, void* entry);
void Process_destroy(Process* self);

#endif