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
        fileNodes[i].fileType = FILETYPE_NORMAL; //TODO decide according to TAR filetype
        fileNodes[i].vfs = &ramdisk;
        fileNodes[i].ptr = 0;

        firstHeader = Tar_nextHeader(firstHeader);
        i++;

        if(firstHeader == NULL)
            break;

    }

}

PRIVATE void RamDisk_open(VFSNode* self) {

    UNUSED(self);

}

PRIVATE void RamDisk_close(VFSNode* self) {

    UNUSED(self);

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

//TODO: Implement
PRIVATE u32int RamDisk_write(VFSNode* self, u32int offset, u32int count, char* buffer) {

    UNUSED(self);
    UNUSED(offset);
    UNUSED(count);
    UNUSED(buffer);
    return 0;

}

//TODO: Implement
PRIVATE DirEntry* RamDisk_readDir(VFSNode* self, u32int index) {

    UNUSED(self);
    UNUSED(index);

    return 0;

}

//TODO: Implement
PRIVATE VFSNode* RamDisk_findDir(VFSNode* self, const char* path) {

    UNUSED(self);
    UNUSED(path);

    return 0;

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

    /* Insert root node */
    String_copy(fileNodes[0].fileName, "initrd");
    fileNodes[0].permission = 0;
    fileNodes[0].uid = 0;
    fileNodes[0].gid = 0;
    fileNodes[0].index = 0; /* First file index */
    fileNodes[0].fileSize = 0;
    fileNodes[0].ptr = 0;
    fileNodes[0].fileType = FILETYPE_DIRECTORY;
    fileNodes[0].vfs = &ramdisk;

    /* Set ramdisk */
    ramdisk.rootNode = &fileNodes[0];
    ramdisk.deviceID = RAMDISK_DEVICE_ID;
    ramdisk.open = RamDisk_open;
    ramdisk.close = RamDisk_close;
    ramdisk.read = RamDisk_read;
    ramdisk.write = RamDisk_write;
    ramdisk.readDir = RamDisk_readDir;
    ramdisk.findDir = RamDisk_findDir;

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