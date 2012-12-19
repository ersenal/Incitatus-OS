/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Process.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Process/Process.h>
#include <Memory/HeapMemory.h>
#include <Memory/VirtualMemory.h>
#include <Memory/PhysicalMemory.h>
#include <X86/GDT.h>

/*=======================================================
    FUNCTION
=========================================================*/

PUBLIC Process* Process_new(u32int id, char* name, void* entry) {

    Process* self = HeapMemory_alloc(sizeof(Process));

    self->kernelStackTop = (Regs*) ((u32int) HeapMemory_alloc(FRAME_SIZE) + FRAME_SIZE - sizeof(Regs));

    self->kernelStackTop->eflags = 0x202;
    self->kernelStackTop->cs = 0x08;
    self->kernelStackTop->eip = (u32int) entry;

    self->kernelStackTop->edi = 0;
    self->kernelStackTop->esi = 0;
    self->kernelStackTop->ebp = 0;
    self->kernelStackTop->esp = 0;
    self->kernelStackTop->ebx = 0;
    self->kernelStackTop->edx = 0;
    self->kernelStackTop->ecx = 0;
    self->kernelStackTop->eax = 0;

    self->kernelStackTop->ds = 0x10;
    self->kernelStackTop->es = 0x10;
    self->kernelStackTop->fs = 0x10;
    self->kernelStackTop->gs = 0x10;

    self->kernelStackTop->esp0 = 1049013;
    self->kernelStackTop->ss0 = 1159456;

    self->entry = entry;
    self->name = name;
    self->pid = id;

    return self;

}

PUBLIC void Process_destroy(Process* self) {

    HeapMemory_free(self->kernelStackTop);
    HeapMemory_free(self);

}