/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| VirtualMemory.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Sets up and manages virtual memory.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef VM_H
#define VM_H

#include <Module.h>

/*-------------------------------------------------------------------------
| Get VMM module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns virtual memory module.
|
\------------------------------------------------------------------------*/
Module* VirtualMemory_getModule(void);
#endif