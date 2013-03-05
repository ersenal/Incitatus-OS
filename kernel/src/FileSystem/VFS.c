/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| VFS.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Virtual File System interface
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <FileSystem/VFS.h>
#include <FileSystem/RamDisk.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module vfsModule;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void VFS_init(void) {

    RamDisk_init();

}

PUBLIC Module* VFS_getModule(void) {

    if(!vfsModule.isLoaded) {

        vfsModule.moduleName = "Virtual File System";
        vfsModule.moduleID = MODULE_VFS;
        vfsModule.init = VFS_init;
        vfsModule.numberOfDependencies = 1;
        vfsModule.dependencies[0] = MODULE_HEAP;

    }

    return &vfsModule;

}