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

/* Include PMM implementation */
/* #include Memory/BitmapPMM.h */
#include <Memory/StackPMM.h>


/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module pmmModule;
PRIVATE MultibootInfo* multibootInfo;
PRIVATE MultibootHeader* multibootHeader;

/*=======================================================
    PUBLIC DATA
=========================================================*/
PUBLIC PhysicalMemoryInfo  (*PhysicalMemory_getInfo) (void);
PUBLIC void* (*PhysicalMemory_allocateFrame) (void);
PUBLIC void  (*PhysicalMemory_freeFrame) (void* frame);

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void _PhysicalMemory_init() {

    /* Point to PMM implementation */
    PhysicalMemory_getInfo       = StackPMM_getInfo;
    PhysicalMemory_allocateFrame = StackPMM_allocateFrame;
    PhysicalMemory_freeFrame     = StackPMM_freeFrame;

    /* Call PMM init function */
    StackPMM_init(multibootInfo, multibootHeader);

}

PUBLIC Module* PhysicalMemory_getModule(MultibootInfo* mbI, MultibootHeader* mbH) {

    if(!pmmModule.isLoaded) {

        pmmModule.moduleName = "Physical Memory Manager";
        pmmModule.moduleID = MODULE_PMM;
        pmmModule.init = _PhysicalMemory_init;

        multibootInfo = mbI;
        multibootHeader = mbH;

    }

    return &pmmModule;
}



