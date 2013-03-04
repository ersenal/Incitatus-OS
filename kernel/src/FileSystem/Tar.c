/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Tar.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Tar archive parser.
|
|               For more info on Tar:
|                   http://en.wikipedia.org/wiki/Tar_file_format
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <FileSystem/Tar.h>
#include <Debug.h>
#include <Lib/String.h>

/* Note: Allocates buffer in kernel heap */
// PRIVATE char* Tar_getFileContents(const TarEntryHeader* header) {

//     Debug_assert(header != NULL);

//     char* loc = ((char*) header) + TAR_BLOCK_SIZE;
//     int fileSize = String_stringToInt(header->fileSize, 8);

//     char* fileContents = HeapMemory_calloc(1, fileSize + 1); /* Adding 1 since we need space for one null char */
//     Memory_copy(fileContents, loc, fileSize);
//     fileContents[fileSize] = '\0'; /* Null terminate our file */

//     return fileContents;

// }

PRIVATE u32int Tar_validateEntry(const TarEntryHeader* header) {

    Debug_assert(header != NULL);

    u8int* address = (u8int*) header;
    u32int checksum = String_stringToInt(header->checksum, 8);

    for (u32int i = 0; i < TAR_BLOCK_SIZE; i++)
        checksum -= (i >= 148 && i < 156) ? 32 : address[i];

    return !checksum;

}

PUBLIC TarEntryHeader* Tar_nextHeader(const TarEntryHeader* header) {

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

PUBLIC u32int Tar_getNumberOfFiles(const TarEntryHeader* firstHeader) {

    Debug_assert(firstHeader != NULL);

    u32int fileCount = 0;

    /* For every file in archive */
    while(TRUE) {

        if (firstHeader->fileName[0] == '\0')
            break;

        firstHeader = Tar_nextHeader(firstHeader);
        fileCount++;

        if(firstHeader == NULL)
            break;

    }

    return fileCount;

}