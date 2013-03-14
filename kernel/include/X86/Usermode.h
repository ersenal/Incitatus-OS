/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Usermode.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Handles ring0 to ring3 switch and syscalls.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef USER_MODE_H
#define USER_MODE_H

#include <Module.h>

/*-------------------------------------------------------------------------
| Get usermode module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the usermode module.
|
| NOTES:           After loading this module, OS executes at ring 3.
\------------------------------------------------------------------------*/
Module* Usermode_getModule(void);

#endif