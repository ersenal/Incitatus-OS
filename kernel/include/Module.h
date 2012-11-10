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
#define MAX_LOADED_MODULES    32
#define MAX_MODULE_DEPENDENCY  8

/* Module IDs */
#define MODULE_VGA     100
#define MODULE_CONSOLE 101
#define MODULE_GDT     102
#define MODULE_IDT     103
#define MODULE_PIC8259 104;

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
| Get loaded module names
|--------------------------------------------------------------------------
| DESCRIPTION:      Fills the specified char* array with loaded module
|                   names.
|
| PARAM:           "buffer"    the buffer to store module names
\------------------------------------------------------------------------*/
void Module_getLoadedModuleNames(char** buffer);

/*-------------------------------------------------------------------------
| Get number of loaded modules
|--------------------------------------------------------------------------
| DESCRIPTION:      Returns the number of loaded modules
|
| RETURN:           "u32int"    the number of loaded modules
\------------------------------------------------------------------------*/
u32int Module_getNumberOfLoadedModules(void);

/*-------------------------------------------------------------------------
| Unload module
|--------------------------------------------------------------------------
| DESCRIPTION:     Unloads a given module.
|
| PARAM:           "module"    the module to unload
\------------------------------------------------------------------------*/
void Module_unload(Module* module);

#endif