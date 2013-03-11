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
#include <Debug.h>
#include <Process/Scheduler.h>
#include <Process/ProcessManager.h>
#include <Memory.h>

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

    Debug_assert(node != NULL);

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

//TODO: append mode, 'a' and 'a+', 'r+', 'w+'
//TODO: open according to current working directory
PUBLIC VFSNode* VFS_openFile(const char* filename, const char* mode) {

    if(String_startsWith(filename, "/"))
        filename++;

    VFSNode* fileNode = VFS_searchForFile(rootFS->rootNode, filename);
    Debug_assert(fileNode->mode == FILE_MODE_NOT_OPEN);

    if(String_compare(mode, "r") == 0)
        fileNode->mode = FILE_MODE_READ;
    else if(String_compare(mode, "w") == 0)
        fileNode->mode = FILE_MODE_WRITE;
    else
        Sys_panic("Invalid file mode");

    /* Add opened file to process' file list */
    Process* currentProcess = Scheduler_getCurrentProcess();

    if(currentProcess != NULL)
        ArrayList_add(currentProcess->fileNodes, fileNode);

    return fileNode;

}

PUBLIC bool VFS_closeFile(VFSNode* file) {

    Debug_assert(file != NULL);
    Debug_assert(file->mode != FILE_MODE_NOT_OPEN); /* File must be open to be closed */

    //TODO:
    //All internal buffers associated with the stream are disassociated from it
    //and flushed: the content of any unwritten output buffer is written and the content of any unread input buffer is discarded.
    file->mode = FILE_MODE_NOT_OPEN;

    /* Remove file from process' file list */
    Process* currentProcess = Scheduler_getCurrentProcess();

    if(currentProcess != NULL)
        ArrayList_remove(currentProcess->fileNodes, file);

    return 0;

}

PUBLIC VFSNode* VFS_getParent(VFSNode* child) {

    Debug_assert(child != NULL);

    int childNameLength = String_length(child->fileName);
    char childName[childNameLength];
    String_copy(childName, child->fileName);

    if(String_countChar(childName, '/') == 0)
        return rootFS->rootNode;

    if(child->fileType == FILETYPE_DIRECTORY) {

        if(String_countChar(childName, '/') == 1)
            return rootFS->rootNode;

        childName[childNameLength - 1] = '\0';

    }

    for(int i = childNameLength - 1; i >= 0; i--) {

         if(childName[i] == '/') {
            childName[i+1] = '\0';
            break;
        }

    }

    return VFS_searchForFile(rootFS->rootNode, childName);

}

PUBLIC VFSNode* VFS_readDir(VFSNode* dir, int index) {

    Debug_assert(dir != NULL);
    return dir->vfs->readDir(dir, index);

}

PUBLIC VFSNode* VFS_changeDirectory(VFSNode* file) {

    Debug_assert(file != NULL);
    Debug_assert(file->fileType == FILETYPE_DIRECTORY);

    Scheduler_getCurrentProcess()->workingDirectory = file;

    return file;

}

PUBLIC char* VFS_getWorkingDirectoryStr(char* buf) {

    Debug_assert(buf != NULL);

    String_copy(buf, Scheduler_getCurrentProcess()->workingDirectory->fileName);

    if(buf[0] == '\0') { /* Root working dir */
        buf[0] = '/';
        buf[1] = '\0';
    }

    return buf;

}

PUBLIC void VFS_getFileStats(VFSNode* file, VFSNode* buf) {

    Memory_copy(buf, file, sizeof(VFSNode));

}

PUBLIC VFSNode* VFS_getWorkingDirectoryPtr(void) {

    return Scheduler_getCurrentProcess()->workingDirectory;

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