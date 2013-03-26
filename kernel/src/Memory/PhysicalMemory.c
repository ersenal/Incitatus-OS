/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| PhysicalMemory.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Physical memory manager interface.
|               Sets up and manages physical memory.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Memory/PhysicalMemory.h>
#include <Debug.h>

/* Include PMM implementation */
/* #include Memory/BitmapPMM.h */
#include <Memory/StackPMM.h>


/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module pmmModule;

/*=======================================================
    PUBLIC DATA
=========================================================*/
PUBLIC PhysicalMemoryInfo*  (*PhysicalMemory_getInfo) (PhysicalMemoryInfo* buf);
PUBLIC void* (*PhysicalMemory_allocateFrame) (void);
PUBLIC void  (*PhysicalMemory_freeFrame) (void* frame);

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void _PhysicalMemory_init() {

    Debug_logInfo("%s%s", "Initialising ", pmmModule.moduleName);

    /* Point to PMM implementation */
    PhysicalMemory_getInfo       = StackPMM_getInfo;
    PhysicalMemory_allocateFrame = StackPMM_allocateFrame;
    PhysicalMemory_freeFrame     = StackPMM_freeFrame;

    /* Call PMM init function */
    StackPMM_init();

}

PUBLIC Module* PhysicalMemory_getModule(void) {

    if(!pmmModule.isLoaded) {

        pmmModule.moduleName = "Physical Memory Manager";
        pmmModule.moduleID = MODULE_PMM;
        pmmModule.init = _PhysicalMemory_init;

    }

    return &pmmModule;
}



