/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| PIT8253.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Provides an interface for Intel 8253 Programmable Interval
|               Timer(PIT).
|
|               PIT consists of three timers(channels). Each has a different
|               purpose.
|
|               First Timer  - Used as the System timer
|               Second Timer - Used for RAM refreshing
|               Third Timer  - Connected to PC speaker(that annoying beeper)
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
|
|               TYPE comments source:
|                   http://stanislavs.org/helppc/8259.html
\------------------------------------------------------------------------*/


#ifndef PIT_H
#define PIT_H

#include <Common.h>
#include <Module.h>

/*-------------------------------------------------------------------------
| Measure runtime
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the number of ticks it takes to compute
|                  specified function.
|
| PARAM:           "functionAddr"  the function to be measured
|
| RETURN:          "u32int"        the number of ticks
\------------------------------------------------------------------------*/
u32int PIT8253_measureRuntime(void* functionAddr);

/*-------------------------------------------------------------------------
| Sleep
|--------------------------------------------------------------------------
| DESCRIPTION:     Puts the entire system on a busy-wait
|
| PARAM:           "ms"  time in milliseconds
\------------------------------------------------------------------------*/
void PIT8253_sleep(u32int ms);

/*-------------------------------------------------------------------------
| Get PIT module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the PIT module.
\------------------------------------------------------------------------*/
Module* PIT8253_getModule(void);
#endif