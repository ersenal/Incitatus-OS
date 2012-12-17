/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| ProcessManager.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef PROCESS_M_H
#define PROCESS_M_H

#include <Module.h>
#include <X86/IDT.h>

Module* ProcessManager_getModule(void);
void  ProcessManager_schedule(Regs* context);

#endif