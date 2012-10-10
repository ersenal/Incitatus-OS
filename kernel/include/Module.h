/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Module.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef MODULE_H
#define MODULE_H

#include <Common.h>

/*=======================================================
    STRUCT
=========================================================*/
typedef struct Module Module;

/*=======================================================
    FUNCTION
=========================================================*/
void Module_load(Module* module);

#endif