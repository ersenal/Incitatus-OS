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
    DEFINE
=========================================================*/

/* Module IDs */
#define MODULE_VGA     100
#define MODULE_CONSOLE 101

#define MAX_MODULE_NAME_CHARS 63
#define MAX_MODULE_DEPENDENCY  8

/*=======================================================
    STRUCT
=========================================================*/
typedef struct Module Module;

struct Module {

    void    (*init) (void);
    bool    isLoaded;
    char*   moduleName;
    u32int  moduleID;
    u32int  dependencies[MAX_MODULE_DEPENDENCY];
    u32int  numberOfDependencies;

};

/*=======================================================
    FUNCTION
=========================================================*/
void Module_load(Module* module);

#endif