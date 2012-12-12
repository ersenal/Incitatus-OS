/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| VirtualMemory.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Platform dependent X86 virtual memory manager, initialiser.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef VMM_H
#define VMM_H

#include <Module.h>

/*-------------------------------------------------------------------------
| Get VMM module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns virtual memory manager module.
|
\------------------------------------------------------------------------*/
Module* VirtualMemory_getModule(void);
#endif