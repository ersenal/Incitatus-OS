/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Sys.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Provides important system functions.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef SYS_H
#define SYS_H

/*-------------------------------------------------------------------------
| Restart CPU
|--------------------------------------------------------------------------
| DESCRIPTION:  Restarts the CPU
|
\------------------------------------------------------------------------*/
void Sys_restart(void);

/*-------------------------------------------------------------------------
| Halt CPU
|--------------------------------------------------------------------------
| DESCRIPTION:     Halts the CPU, the system becomes idle.
|
| NOTES:           Interrupts wake up the CPU.
\------------------------------------------------------------------------*/
void Sys_haltCPU(void);

/*-------------------------------------------------------------------------
| Kernel panic
|--------------------------------------------------------------------------
| DESCRIPTION:     Throws a kernel panic message, disables interrupts
|                  and halts the CPU.
|
| PARAM:           "str"    the panic message
|
| NOTES:           System becomes unresponsive after this call.
\------------------------------------------------------------------------*/
void Sys_panic(const char* str);
#endif
