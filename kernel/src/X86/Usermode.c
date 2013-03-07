/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Usermode.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Handles ring0 to ring3 switch.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <X86/Usermode.h>
#include <Common.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module userModule;
PRIVATE void*  usermodeEntryAddr;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void Usermode_init(void) {

    asm volatile("      \
      cli;              \
      mov $0x23, %ax;   \
      mov %ax, %ds;     \
      mov %ax, %es;     \
      mov %ax, %fs;     \
      mov %ax, %gs;     \
                        \
      mov %esp, %eax;   \
      pushl $0x23;      \
      pushl %esp;       \
      pushf;            \
      pop %eax;         \
      or $0x200, %eax;  \
      push %eax;        \
      pushl $0x1B;      \
      push $1f;         \
      iret;             \
    1:                  \
      ");

    void (*jumpTo) (void) = usermodeEntryAddr;
    jumpTo();

}

PUBLIC Module* Usermode_getModule(void* usermodeEntry) {

    if(!userModule.isLoaded) {

        userModule.moduleName = "Usermode";
        userModule.init = &Usermode_init;
        userModule.moduleID = MODULE_USERMODE;
        userModule.numberOfDependencies = 1;
        userModule.dependencies[0] = MODULE_PROCESS;

        usermodeEntryAddr = usermodeEntry;

    }

    return &userModule;
}