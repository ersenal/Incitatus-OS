#ifndef MUTEX_H
#define MUTEX_H

#include <Sys.h>

/*-------------------------------------------------------------------------
| Mutex
|--------------------------------------------------------------------------
| DESCRIPTION:  Lock or unlock access to a critical section.
|
| NOTES:        Current implementation exploits(assumes) the fact that
|               we have only one executing CPU.
|
\------------------------------------------------------------------------*/
static inline void Mutex_lock() {

    Sys_disableInterrupts();

}

static inline void Mutex_unlock() {

    Sys_enableInterrupts();

}

#endif