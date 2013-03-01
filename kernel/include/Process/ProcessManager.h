/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| ProcessManager.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Handles context switch.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef PROCESS_M_H
#define PROCESS_M_H

#include <Module.h>
#include <X86/IDT.h>


void  ProcessManager_switch(Regs* context);
Module* ProcessManager_getModule(void);

#endif