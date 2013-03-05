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
#include <Lib/String.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module vfsModule;
PRIVATE VFS*   rootFS;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void VFS_init(void) {

    rootFS = RamDisk_init();

}

PRIVATE VFSNode* VFS_searchForFile(VFSNode* node, const char* filename) {

    VFSNode* n;
    u32int i = 0;

    while((n = node->vfs->readDir(node, i))) {

        if(String_compare(n->fileName, filename) == 0)
            return n;

        if(n->fileType == FILETYPE_DIRECTORY)
            return VFS_searchForFile(n, filename);

        i++;
    }

    return NULL;

}

//TODO: add flags parameter for read/write modes
//TODO: open according to current working directory
PUBLIC VFSNode* VFS_openFile(const char* filename) {

    char* f = (char*) filename;

    if(String_startsWith(filename, "/"))
        f++;

    return VFS_searchForFile(rootFS->rootNode, f);

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