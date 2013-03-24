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
#include <FileSystem/Tar.h>
#include <Memory.h>
#include <Lib/String.h>
#include <Memory/HeapMemory.h>
#include <Multiboot.h>

/*=======================================================
    DEFINE
=========================================================*/
#define ROOT_INDEX  -1
#define RAMDISK_DEVICE_ID 0xBEEF

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE u32int firstHeaderAddress;
PRIVATE u32int numberOfFiles;
PRIVATE VFS ramdisk;
PRIVATE VFSNode  root;
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
        fileNodes[i].vfs = &ramdisk;
        fileNodes[i].ptr = 0;
        fileNodes[i].mode = FILE_MODE_NOT_OPEN;

        switch(firstHeader->fileType[0]) {

            case TAR_FILETYPE_NORMAL:
                fileNodes[i].fileType = FILETYPE_NORMAL;
                break;

            case TAR_FILETYPE_DIRECTORY:
                fileNodes[i].fileType = FILETYPE_DIRECTORY;
                break;

            case TAR_FILETYPE_NAMED_PIPE:
                fileNodes[i].fileType = FILETYPE_PIPE;
                break;

            case TAR_FILETYPE_HARD_LINK:
            case TAR_FILETYPE_SYM_LINK:
                fileNodes[i].fileType = FILETYPE_SOFT_SYMLINK;
                break;

            case TAR_FILETYPE_CHAR_DEVICE:
                fileNodes[i].fileType = FILETYPE_CHAR_DEVICE;
                break;

            case TAR_FILETYPE_BLOCK_DEVICE:
                fileNodes[i].fileType = FILETYPE_BLOCK_DEVICE;
                break;

            default:
                Debug_logWarning("%s", "Unknown Tar filetype");
        }

        firstHeader = Tar_nextHeader(firstHeader);
        i++;

        if(firstHeader == NULL)
            break;

    }

}

PRIVATE u32int RamDisk_read(VFSNode* self, u32int offset, u32int count, char* buffer) {

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

PUBLIC VFS* RamDisk_init(void) {

    /* Get initrd tar module address */
    extern MultibootInfo* multibootInfo; /* Defined in Kernel.c */
    firstHeaderAddress = *((u32int*) multibootInfo->modsAddr);

    /* Reserve space in heap for virtual file nodes */
    numberOfFiles = Tar_getNumberOfFiles((TarEntryHeader*) firstHeaderAddress);
    fileNodes = HeapMemory_calloc(numberOfFiles, sizeof(VFSNode));

    /* Create root */
    root.permission = 0;
    root.uid = 0;
    root.gid = 0;
    root.fileSize = 0;
    root.index = ROOT_INDEX;
    root.fileType = FILETYPE_DIRECTORY;
    root.vfs = &ramdisk;
    root.ptr = 0;
    root.mode = FILE_MODE_NOT_OPEN;

    /* Set ramdisk */
    ramdisk.rootNode = &root;
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

    return &ramdisk;

}