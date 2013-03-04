/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| RamDisk.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Tar-based ramdisk(RAM as disk drive) implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <FileSystem/RamDisk.h>
#include <FileSystem/VFS.h>
#include <FileSystem/Tar.h>
#include <Memory.h>
#include <Lib/String.h>
#include <Memory/HeapMemory.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module ramdiskModule;

PRIVATE u32int firstHeaderAddress;
PRIVATE u32int numberOfFiles;
PRIVATE VFS ramdisk;
PRIVATE VFSNode* fileNodes;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void RamDisk_parseArchive(const TarEntryHeader* firstHeader) {

    Debug_assert(firstHeader != NULL);
    u32int i = 0;

    /* For every file in archive */
    while(TRUE) {

        if (firstHeader->fileName[0] == '\0')
            break;

        /* Add tar file as a virtual file */
        String_copy(fileNodes[i].fileName, firstHeader->fileName);
        fileNodes[i].permission = 0;
        fileNodes[i].uid = 0;
        fileNodes[i].gid = 0;
        fileNodes[i].fileSize = String_stringToInt(firstHeader->fileSize, 8) ;
        fileNodes[i].index = i;

        //TODO Also add other filetypes
        if(firstHeader->fileType[0] == TAR_FILETYPE_NORMAL)
            fileNodes[i].fileType = FILETYPE_NORMAL;
        else if(firstHeader->fileType[0] == TAR_FILETYPE_DIRECTORY)
            fileNodes[i].fileType = FILETYPE_DIRECTORY;
        else {
            Debug_logWarning("%s", "Unknown Tar filetype");
        }

        fileNodes[i].vfs = &ramdisk;
        fileNodes[i].ptr = 0;

        firstHeader = Tar_nextHeader(firstHeader);
        i++;

        if(firstHeader == NULL)
            break;

    }

}

PRIVATE u32int RamDisk_read(VFSNode* self, u32int offset, u32int count, char* buffer) {

    Debug_assert(self != NULL);
    Debug_assert(buffer != NULL);
    Debug_assert(self->vfs != NULL); /* Ensure we have a valid node */
    Debug_assert(offset + count <= self->fileSize); /* Valid boundaries? */
    Debug_assert(self->fileType == FILETYPE_NORMAL); //TODO: make a proper check

    /* Get nth tar header */
    TarEntryHeader* header = Tar_getHeader((TarEntryHeader*) firstHeaderAddress, self->index);
    Debug_assert(header != NULL);

    char* loc = ((char*) header) + TAR_BLOCK_SIZE + offset;
    Memory_copy(buffer, loc, count);
    buffer[count] = '\0'; /* Null terminate our file */

    return count;

}

/* Return nth child of a directory */
PRIVATE VFSNode* RamDisk_readDir(VFSNode* self, u32int index) {

    Debug_assert(self != NULL);
    Debug_assert(self->vfs != NULL); /* Ensure we have a valid node */
    Debug_assert(index < numberOfFiles);
    Debug_assert(self->fileType == FILETYPE_DIRECTORY);

    u32int nthChild = -1;

    /* For every file in ramdisk */
    for(u32int i = 0; i < numberOfFiles; i++) {

        /* Find out the depth of the child file and parent folder */
        u32int childDepth = String_countChar(fileNodes[i].fileName, '/');
        u32int dirDepth = String_countChar(self->fileName, '/');

        if(String_startsWith(fileNodes[i].fileName, self->fileName) && childDepth == dirDepth && fileNodes[i].index != self->index) /* Non-directory file */
            nthChild++;
        else if(String_startsWith(fileNodes[i].fileName, self->fileName) && childDepth == dirDepth + 1 && String_endsWith(fileNodes[i].fileName, "/")) /* Directory file */
            nthChild++;

        if(nthChild == index)
            return &fileNodes[i];

    }

    return NULL;

}

/* Find child file by absolute path in a directory */
PRIVATE VFSNode* RamDisk_findDir(VFSNode* self, const char* path) {

    UNUSED(self);
    Debug_assert(self != NULL);
    Debug_assert(self->vfs != NULL); /* Ensure we have a valid node */

    for(u32int i = 0; i < numberOfFiles; i++)
        if(String_compare(fileNodes[i].fileName, path) == 0)
            return &fileNodes[i];

    return NULL;

}

PRIVATE void RamDisk_test(void) {

    Debug_logInfo("%s", "Testing ramdisk...");

    for(u32int i = 0; i < numberOfFiles; i++) {
        Debug_logInfo("%s%s%c%d%s", "Found: ", fileNodes[i].fileName, ',', fileNodes[i].fileSize, "Bytes");
    }

}

PRIVATE void RamDisk_init(void) {

    /* Reserve space in heap for virtual file nodes */
    numberOfFiles = Tar_getNumberOfFiles((TarEntryHeader*) firstHeaderAddress);
    fileNodes = HeapMemory_calloc(numberOfFiles, sizeof(VFSNode));

    /* Set ramdisk */
    ramdisk.rootNode = &fileNodes[0];
    ramdisk.deviceID = RAMDISK_DEVICE_ID;
    ramdisk.readDir = RamDisk_readDir;
    ramdisk.findDir = RamDisk_findDir;
    ramdisk.read = RamDisk_read;
    ramdisk.write = NULL; /* our ramdisk does not support 'write' */
    ramdisk.open = NULL; /* our ramdisk does not need 'open' */
    ramdisk.close = NULL; /* our ramdisk does not need 'close' */

    /* Parse tar archive and test ramdisk */
    RamDisk_parseArchive((TarEntryHeader*) firstHeaderAddress);
    RamDisk_test();

}

PUBLIC Module* RamDisk_getModule(u32int firstHeaderAddr) {

    if(!ramdiskModule.isLoaded) {

        ramdiskModule.moduleName = "Ram Disk";
        ramdiskModule.moduleID = MODULE_RAM_DISK;
        ramdiskModule.init = RamDisk_init;
        ramdiskModule.numberOfDependencies = 1;
        ramdiskModule.dependencies[0] = MODULE_HEAP;

        firstHeaderAddress = firstHeaderAddr;

    }

    return &ramdiskModule;

}