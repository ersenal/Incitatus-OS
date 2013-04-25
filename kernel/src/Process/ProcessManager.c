/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| ProcessManager.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Handles process creation, destruction, switch.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Process/ProcessManager.h>
#include <Process/Scheduler.h>
#include <Drivers/Console.h>
#include <Memory/VirtualMemory.h>
#include <Memory/HeapMemory.h>
#include <Memory/PhysicalMemory.h>
#include <Memory.h>
#include <Lib/String.h>
#include <X86/GDT.h>
#include <Process/Mutex.h>

/*=======================================================
    DEFINE
=========================================================*/
#define PROCESS_MSG_WAITING 0x100

/*=======================================================
    STRUCT
=========================================================*/
typedef struct Message Message;

struct Message {

    Process* to;
    Process* from;
    u32int   message;

};

/*=======================================================
    PUBLIC DATA
=========================================================*/
PUBLIC Process*  kernelProcess;

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module     pmModule;
PRIVATE u32int     pid;
PRIVATE ArrayList* globalMailbox;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE Process* ProcessManager_newProcess(void) {

    Process* self = HeapMemory_calloc(1, sizeof(Process));
    Debug_assert(self != NULL);

    self->pid = pid;
    self->userHeapTop = (void*) USER_HEAP_BASE_VADDR;
    self->fileNodes = ArrayList_new(1);

    Regs registers;
    Memory_set(&registers, 0, sizeof(Regs));

    registers.eflags = 0x202; /* Interrupt enable flag */
    registers.eip    = USER_CODE_BASE_VADDR; /* Initial code entry point */
    registers.intNo  = IRQ0;

    /* Add 3 so that they have an RPL of 3 (User ring) */
    registers.cs = USER_CODE_SEGMENT | 3;
    registers.ds = USER_DATA_SEGMENT | 3;
    registers.es = USER_DATA_SEGMENT | 3;
    registers.fs = USER_DATA_SEGMENT | 3;
    registers.gs = USER_DATA_SEGMENT | 3;

    /* Create a new page directory for process */
    VirtualMemory_createPageDirectory(self);

    /* Map kernel bottom 4MB + kernel heap */
    VirtualMemory_mapKernel(self);

    /* Allocate kernel stack - 4KB */
    u32int* stack = HeapMemory_calloc(1, FRAME_SIZE);
    Debug_assert(stack != NULL);
    self->kernelStackBase = stack;
    self->kernelStack = (char*) stack + FRAME_SIZE;

    /* Allocate and map user stack - Currently 4KB */
    Debug_assert(USER_STACK_SIZE == FRAME_SIZE);
    self->userStackBase = (void*) USER_STACK_BASE_VADDR;
    char* uStack = PhysicalMemory_allocateFrame();

    VirtualMemory_mapPage(self->pageDir, (void*) USER_STACK_BASE_VADDR, uStack, MODE_USER);
    self->userStack = (void*) ((char*) self->userStackBase + FRAME_SIZE - sizeof(Regs));

    /* Set up initial user ss and esp */
    registers.esp0   = (u32int) self->userStack;
    registers.ss0    = USER_DATA_SEGMENT | 3;

    if(pid == 1) { /* Initial user process, prevent page fault by NOT unmapping while entering user mode */

        VirtualMemory_quickMap((void*) USER_STACK_BASE_VADDR, uStack);
        Memory_copy(self->userStack, &registers, sizeof(Regs));

    } else {

        VirtualMemory_quickMap((void*) TEMPORARY_MAP_VADDR, uStack);
        Memory_copy((void*) (TEMPORARY_MAP_VADDR + FRAME_SIZE - sizeof(Regs)), &registers, sizeof(Regs));
        VirtualMemory_quickUnmap((void*) TEMPORARY_MAP_VADDR);

    }

    pid++;
    self->status = PROCESS_CREATED;

    return self;

}

PRIVATE void ProcessManager_initKernelProcess(void) {

    extern void Kernel_idle(void); /* Defined in Kernel.c */
    kernelProcess = HeapMemory_calloc(1, sizeof(Process));
    Debug_assert(kernelProcess != NULL);

    kernelProcess->pid = 0;
    kernelProcess->fileNodes = NULL;
    kernelProcess->userHeapTop = NULL;
    kernelProcess->userStackBase = NULL;
    kernelProcess->userStack = NULL;
    kernelProcess->workingDirectory = NULL;
    String_copy(kernelProcess->name, "Idle");

    Regs registers;
    Memory_set(&registers, 0, sizeof(Regs));

    registers.eflags = 0x202; /* Interrupt enable flag */
    registers.eip    = (u32int) &Kernel_idle; /* Initial code entry point */
    registers.intNo  = IRQ0;

    registers.cs   = KERNEL_CODE_SEGMENT;
    registers.ds   = KERNEL_DATA_SEGMENT;
    registers.es   = KERNEL_DATA_SEGMENT;
    registers.fs   = KERNEL_DATA_SEGMENT;
    registers.gs   = KERNEL_DATA_SEGMENT;
    registers.esp0 = 0;
    registers.ss0  = 0;

    /* Set page directory */
    kernelProcess->pageDir = VirtualMemory_getKernelDir();

    /* Allocate kernel stack - 4KB */
    u32int* stack = HeapMemory_calloc(1, FRAME_SIZE);
    Debug_assert(stack != NULL);
    kernelProcess->kernelStackBase = stack;
    kernelProcess->kernelStack = (char*) stack + FRAME_SIZE - sizeof(Regs);
    Memory_copy(kernelProcess->kernelStack, &registers, sizeof(Regs));

    kernelProcess->status = PROCESS_CREATED;

}

PRIVATE void ProcessManager_destroyProcess(Process* process) {

    HeapMemory_free(process->kernelStackBase);

    /* close all opened files */
    for(u32int i = 0; i < ArrayList_getSize(process->fileNodes); i++) {

        VFSNode* file = ArrayList_get(process->fileNodes, i);
        VFS_closeFile(file);

    }

    ArrayList_destroy(process->fileNodes);
    VirtualMemory_destroyPageDirectory(process);
    HeapMemory_free(process);

}

PRIVATE void ProcessManager_notify(void) {

    for(u32int i = 0; i < ArrayList_getSize(globalMailbox); i++) {

        Message* m = ArrayList_get(globalMailbox, i);

        if(m->to == Scheduler_getCurrentProcess()) {

            if(m->message == PROCESS_MSG_WAITING)
                m->from->status = PROCESS_WAITING;

            ArrayList_remove(globalMailbox, m);
            HeapMemory_free(m);

        }

    }

}

PRIVATE void ProcessManager_forceSwitch(void) {

    Sys_enableInterrupts();
    asm volatile("int %0" : : "i" (IRQ0)); /* Force task switch */

}

PRIVATE void ProcessManager_init(void) {

    Debug_logInfo("%s%s", "Initialising ", pmModule.moduleName);

    pid = 1; /* User process pids are >= 1 */
    Scheduler_init();
    globalMailbox = ArrayList_new(10);
    ProcessManager_initKernelProcess();

}

PUBLIC void ProcessManager_switch(Regs* context) {

    Process* currentProcess = Scheduler_getCurrentProcess();
    Debug_assert(currentProcess != NULL);

    if(currentProcess == kernelProcess) { /* Switching from kernel process */

        currentProcess->kernelStack = context; /* Save process state */
        currentProcess->status = PROCESS_WAITING;

    } else { /* Switching from a user process */

        if(currentProcess->status == PROCESS_TERMINATED) { /* Switching from a *killed* user process, free resources */

            ProcessManager_destroyProcess(currentProcess);

        } else {

            if(Scheduler_isPreemptive()) {

                if(currentProcess->status != PROCESS_BLOCKED)
                    currentProcess->status = PROCESS_WAITING;

            } else {

                currentProcess->status = PROCESS_WAITING;

            }

            currentProcess->userStack = context; /* Save process state */

        }

    }

    /* Get next process from scheduler */
    Process* next = Scheduler_getNextProcess();
    Debug_assert(next != NULL);

    while(next->status == PROCESS_BLOCKED) /* Find a waiting process */
        next = Scheduler_getNextProcess();

    Debug_assert(next->status == PROCESS_WAITING);
    next->status = PROCESS_RUNNING;
    if(currentProcess == next) /* No need for a context switch */
        return;


    Debug_assert(next->kernelStack != NULL);

    if(next == kernelProcess) { /* Next process is kernel process */

        asm volatile("mov %0, %%DR0" : : "r" (next->kernelStack)); /* Store kernel process' ESP in DR0 register */

    } else { /* Next process is a user process */

        Debug_assert(next->userStack != NULL);

        GDT_setTSS(KERNEL_DATA_SEGMENT, (u32int) next->kernelStack);
        asm volatile("mov %0, %%DR0" : : "r" (next->userStack)); /* Store next process' ESP in DR0 register */

    }

    VirtualMemory_switchPageDir(next->pageDir); /* Switch to next process' address space */

}

PUBLIC void ProcessManager_killProcess(int exitCode) {

    UNUSED(exitCode);

    Process* current = Scheduler_getCurrentProcess();
    Debug_assert(current != NULL);
    Debug_assert(current != kernelProcess); /* Can't kill kernel process */
    Debug_logInfo("%s%d%c%s%s%d", "PID:", current->pid, ' ', current->name, " exited with code ", exitCode);

    ProcessManager_notify();
    Scheduler_removeProcess(current);

    ProcessManager_forceSwitch();
    while(1); /* Should not reach here */

}

PUBLIC Process* ProcessManager_spawnProcess(const char* binary) {

    VFSNode* bin = VFS_openFile(binary, "r");
    if(bin == NULL) /* Couldn't open the file */
        return NULL;

    char* buffer = HeapMemory_calloc(1, bin->fileSize + 1);
    Debug_assert(buffer != NULL);
    VFS_read(bin, 0, bin->fileSize, buffer);

    Process* p = ProcessManager_newProcess();
    p->workingDirectory = VFS_getParent(bin);
    String_copy(p->name, bin->fileName); /* Set process name */
    Debug_assert(p != NULL);

    u32int i;

    /* VirtualMemory_mapPage uses TEMPORARY_MAP_VADDR and TEMPORARY_MAP_VADDR + 0x1000 */
    /* This could interfere with our VirtualMemory_quickMap, so use a higher temporary map address */
    u32int tempMapAddr = TEMPORARY_MAP_VADDR + (2 * FRAME_SIZE);

    /* Copy user code from kernel heap to user space */
    for(i = 0; i < (bin->fileSize / FRAME_SIZE) + 1; i++) {

        void* phys = PhysicalMemory_allocateFrame();
        VirtualMemory_mapPage(p->pageDir, (void*) (USER_CODE_BASE_VADDR + (i * FRAME_SIZE)), phys, MODE_USER);
        VirtualMemory_quickMap((void*) (tempMapAddr + (i * FRAME_SIZE)), phys);

    }

    Memory_copy((void*) tempMapAddr, buffer, bin->fileSize);

    for(u32int y = 0; y < i; y++)
        VirtualMemory_quickUnmap((void*) (tempMapAddr + (y * FRAME_SIZE)));

    HeapMemory_free(buffer);
    VFS_closeFile(bin);
    Scheduler_addProcess(p);

    return p;

}

PUBLIC void ProcessManager_blockCurrentProcess(void) {

    Scheduler_getCurrentProcess()->status = PROCESS_BLOCKED;
    ProcessManager_forceSwitch();

}

PUBLIC void ProcessManager_waitPID(Process* process) {

    Debug_assert(process != NULL);

    Message* m = HeapMemory_calloc(1, sizeof(Message));

    /* Set current process as waiting for parameter process' termination */
    m->from = Scheduler_getCurrentProcess();
    m->to = process;
    m->message = PROCESS_MSG_WAITING;
    ArrayList_add(globalMailbox, m);
    ProcessManager_blockCurrentProcess();

}

PUBLIC Module* ProcessManager_getModule(void) {

    if(!pmModule.isLoaded) {

        pmModule.moduleName = "Process Manager";
        pmModule.moduleID   = MODULE_PROCESS;
        pmModule.init       = &ProcessManager_init;
        pmModule.numberOfDependencies = 1;
        pmModule.dependencies[0] = MODULE_VFS;

    }

    return &pmModule;

}