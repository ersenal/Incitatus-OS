/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Usermode.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Handles ring0 to ring3 switch and syscalls.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <X86/Usermode.h>
#include <Common.h>
#include <Debug.h>
#include <X86/IDT.h>
#include <X86/GDT.h>
#include <Process/ProcessManager.h>
#include <Memory/VirtualMemory.h>
#include <Memory/HeapMemory.h>
#include <Drivers/Keyboard.h>

/*=======================================================
    DEFINE
=========================================================*/
#define SYSCALL_INTERRUPT   0x80
#define NUMBER_OF_CALLS       23

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module userModule;

PRIVATE void* syscalls[NUMBER_OF_CALLS] = {

    &Console_printString,
    &Console_printChar,
    &ProcessManager_killProcess,
    &ProcessManager_spawnProcess,
    &VFS_changeDirectoryPtr,
    &VFS_getParent,
    &VFS_getWorkingDirectoryStr,
    &VFS_readDir,
    &VFS_getWorkingDirectoryPtr,
    &VFS_getFileStats,
    &Keyboard_getChar,
    &Console_clearScreen,
    &Sys_restart,
    &VFS_openFile,
    &VFS_closeFile,
    &VFS_read,
    &VFS_write,
    &VFS_mkdir,
    &HeapMemory_expandUser,
    &VFS_findDir,
    &VFS_changeDirectoryStr,
    &Console_setColor,
    &Sys_powerOff,

};

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void Usermode_syscallHandler(Regs* regs) {

    /* Valid call request? */
    Debug_assert(regs->eax < NUMBER_OF_CALLS);

    int ret = 0;

    asm volatile("\
        push %1;   \
        push %2;   \
        push %3;   \
        push %4;   \
        push %5;   \
        call *%6;  \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
    " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (syscalls[regs->eax]));

    regs->eax = ret; /* Store return value in eax */

}

PRIVATE void Usermode_init(void) {

    Debug_logInfo("%s", "Jumping to user space");
    IDT_registerHandler(&Usermode_syscallHandler, SYSCALL_INTERRUPT);

    Process* init = ProcessManager_spawnProcess("/Shell");
    GDT_setTSS(KERNEL_DATA_SEGMENT, (u32int) init->kernelStack);
    asm volatile("mov %0, %%esp" : : "r" (init->userStack));
    VirtualMemory_switchPageDir(init->pageDir);

    asm volatile("          \
        cli;                \
        mov $0x23, %%ax;    \
        mov %%ax, %%ds;     \
        mov %%ax, %%es;     \
        mov %%ax, %%fs;     \
        mov %%ax, %%gs;     \
                            \
        mov %%esp, %%eax;   \
        pushl $0x23;        \
        pushl %%eax;        \
        pushf;              \
        pop %%eax;          \
        or $0x200, %%eax;   \
        push %%eax;         \
        pushl $0x1B;        \
        push  %0;           \
        iret;               \
    " : : "S" (USER_CODE_BASE_VADDR));

}

PUBLIC Module* Usermode_getModule(void) {

    if(!userModule.isLoaded) {

        userModule.moduleName = "Usermode";
        userModule.init = &Usermode_init;
        userModule.moduleID = MODULE_USERMODE;
        userModule.numberOfDependencies = 1;
        userModule.dependencies[0] = MODULE_SCHEDULER;

    }

    return &userModule;
}