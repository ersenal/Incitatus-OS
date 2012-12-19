/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Mutex.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Lock or unlock access to a critical section.
|
| NOTES:        Current implementation exploits(assumes) the fact that
|               we have only one executing CPU.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef MUTEX_H
#define MUTEX_H

#include <Sys.h>

static inline void Mutex_lock() {

    Sys_disableInterrupts();

}

static inline void Mutex_unlock() {

    Sys_enableInterrupts();

}

#endif