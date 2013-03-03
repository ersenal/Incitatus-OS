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
|               For more info on Tar:
|                   http://en.wikipedia.org/wiki/Tar_file_format
|
|               With thanks to Brendan Long:
|                   http://stackoverflow.com/questions/2505042/how-to-parse-a-tar-file-in-c
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <FileSystem/RamDisk.h>
#include <FileSystem/VFS.h>
#include <Memory.h>
#include <Lib/String.h>
#include <Lib/ArrayList.h>
#include <Memory/HeapMemory.h>

/*=======================================================
    DEFINE
=========================================================*/

#define TAR_BLOCK_SIZE                  512

#define TAR_FILE_NAME_SIZE              100
#define TAR_FILE_MODE_SIZE              8
#define TAR_UID_SIZE                    8
#define TAR_GID_SIZE                    8
#define TAR_FILE_SIZE_SIZE              12
#define TAR_LAST_MODIFIED_SIZE          12
#define TAR_CHECKSUM_SIZE               8
#define TAR_TYPEFLAG_SIZE               1
#define TAR_LINKNAME_SIZE               100

#define TAR_FILETYPE_NORMAL             '0'
#define TAR_FILETYPE_HARD_LINK          '1'
#define TAR_FILETYPE_SYM_LINK           '2'
#define TAR_FILETYPE_DEVICE             '3'
#define TAR_FILETYPE_BLOCK_DEVICE       '4'
#define TAR_FILETYPE_DIRECTORY          '5'
#define TAR_FILETYPE_NAMED_PIPE         '6'

/*=======================================================
    STRUCT
=========================================================*/
typedef struct TarEntryHeader TarEntryHeader;

struct TarEntryHeader {

    char fileName       [TAR_FILE_NAME_SIZE];
    char fileMode       [TAR_FILE_MODE_SIZE];
    char userID         [TAR_UID_SIZE];
    char groupID        [TAR_GID_SIZE];
    char fileSize       [TAR_FILE_SIZE_SIZE];
    char lastModified   [TAR_LAST_MODIFIED_SIZE];
    char checksum       [TAR_CHECKSUM_SIZE];
    char fileType       [TAR_TYPEFLAG_SIZE];
    char linkName       [TAR_LINKNAME_SIZE];

};

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE u32int firstHeader;
PRIVATE Module ramdiskModule;
PRIVATE VFS ramdisk;
PRIVATE ArrayList* tarHeaders;

/*=======================================================
    FUNCTION
=========================================================*/

/* Note: Allocates buffer in kernel heap */
PUBLIC char* Tar_getFileContents(const TarEntryHeader* header) {

    Debug_assert(header != NULL);

    char* loc = ((char*) header) + TAR_BLOCK_SIZE;
    int fileSize = String_stringToInt(header->fileSize, 8);

    char* fileContents = HeapMemory_calloc(1, fileSize + 1); /* Adding 1 since we need space for one null char */
    Memory_copy(fileContents, loc, fileSize);
    fileContents[fileSize] = '\0'; /* Null terminate our file */

    return fileContents;

}

PRIVATE u32int Tar_validateEntry(const TarEntryHeader* header) {

    Debug_assert(header != NULL);

    u8int* address = (u8int*) header;
    u32int checksum = String_stringToInt(header->checksum, 8);

    for (u32int i = 0; i < TAR_BLOCK_SIZE; i++)
        checksum -= (i >= 148 && i < 156) ? 32 : address[i];

    return !checksum;

}

PRIVATE TarEntryHeader* Tar_nextHeader(const TarEntryHeader* header) {

    Debug_assert(header != NULL);

    u32int fileSize = String_stringToInt(header->fileSize, 8);
    u32int address = (u32int) header + (((fileSize / 512) + 1) * 512);

    if (fileSize % 512)
        address += 512;

    header = (TarEntryHeader*) address;

    if(Tar_validateEntry(header))
        return (TarEntryHeader*) header;

    return NULL;

}

/* Returns the number of files in the TAR archive */
PRIVATE u32int Tar_parseArchive(const TarEntryHeader* firstHeader) {

    Debug_assert(firstHeader != NULL);

    u32int fileCount = 0;

    while(TRUE) {

        if (firstHeader->fileName[0] == '\0')
            break;

        ArrayList_add(tarHeaders, (void*) firstHeader);
        firstHeader = Tar_nextHeader(firstHeader);
        fileCount++;

        if(firstHeader == NULL)
            break;

    }

    return fileCount;

}

PRIVATE u32int RamDisk_getPhysicalAddr(VFS *self, u32int id) {

    UNUSED(self);
    return id + TAR_BLOCK_SIZE;

}

PRIVATE u32int RamDisk_open(VFS *self, u32int id) {

    UNUSED(self);
    return id;

}

PRIVATE u32int RamDisk_close(VFS *self, u32int id) {

    UNUSED(self);
    return id;

}

//TODO: Implement
PRIVATE u32int RamDisk_read(VFS* self, u32int id, u32int offset, u32int count, void* buffer) {

    UNUSED(self);
    UNUSED(id);
    UNUSED(offset);
    UNUSED(count);
    UNUSED(buffer);
    return 0;

}

//TODO: Implement
PRIVATE u32int RamDisk_write(VFS* self, u32int id, u32int offset, u32int count, void* buffer) {

    UNUSED(self);
    UNUSED(id);
    UNUSED(offset);
    UNUSED(count);
    UNUSED(buffer);
    return 0;

}

//TODO: Implement
PRIVATE u32int RamDisk_walk(VFS* self, u32int id, u32int count, const char* path) {

    UNUSED(self);
    UNUSED(id);
    UNUSED(count);
    UNUSED(path);
    return 0;

}

PRIVATE void RamDisk_init(void) {

    ramdisk.rootID = (u32int) firstHeader;
    ramdisk.close = RamDisk_close;
    ramdisk.open = RamDisk_open;
    ramdisk.getPhysicalAddr = RamDisk_getPhysicalAddr;
    ramdisk.read = RamDisk_read;
    ramdisk.write = RamDisk_write;
    ramdisk.walk = RamDisk_walk;

    tarHeaders = ArrayList_new(32);
    Tar_parseArchive((TarEntryHeader*) firstHeader);

    char * s = Tar_getFileContents((TarEntryHeader*) ArrayList_get(tarHeaders, 0));
    Console_printString(s);

}

PUBLIC Module* RamDisk_getModule(u32int firstHeaderAddr) {

    if(!ramdiskModule.isLoaded) {

        ramdiskModule.moduleName = "Ram Disk";
        ramdiskModule.moduleID = MODULE_RAM_DISK;
        ramdiskModule.init = RamDisk_init;
        ramdiskModule.numberOfDependencies = 1;
        ramdiskModule.dependencies[0] = MODULE_HEAP;

        firstHeader = firstHeaderAddr;

    }

    return &ramdiskModule;

}