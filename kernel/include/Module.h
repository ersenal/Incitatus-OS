/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Module.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Manages OS modules(components).
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef MODULE_H
#define MODULE_H

#include <Common.h>

/*=======================================================
    DEFINE
=========================================================*/

#define MAX_MODULE_DEPENDENCY  8

/* Module IDs */
#define MODULE_VGA     100
#define MODULE_CONSOLE 101

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

/*-------------------------------------------------------------------------
| Load module
|--------------------------------------------------------------------------
| DESCRIPTION:     Loads a given module.
|
| PARAM:           "module"    the module to load
\------------------------------------------------------------------------*/
void Module_load(Module* module);

/*-------------------------------------------------------------------------
| Unload module
|--------------------------------------------------------------------------
| DESCRIPTION:     Unloads a given module.
|
| PARAM:           "module"    the module to unload
\------------------------------------------------------------------------*/
void Module_unload(Module* module);

#endif