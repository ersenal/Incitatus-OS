/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Tar.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Tar archive parser.
|
|               For more info on Tar:
|                   http://en.wikipedia.org/wiki/Tar_file_format
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef TAR_H
#define TAR_H

#include <Common.h>

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
#define TAR_FILETYPE_CHAR_DEVICE        '3'
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
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Get next tar header
|--------------------------------------------------------------------------
| DESCRIPTION:  Returns the next tar header.
|
| PARAM:        'header' current header
|
| RETURN        'TarEntryHeader*' the next header
\------------------------------------------------------------------------*/
TarEntryHeader* Tar_nextHeader(const TarEntryHeader* header);

/*-------------------------------------------------------------------------
| Get tar header
|--------------------------------------------------------------------------
| DESCRIPTION:  Returns the nth tar header.
|
| PARAM:        'firstHeader' the first tar header
|               'index'       index of tar header to be returned
|
| RETURN        'TarEntryHeader*' nth tar header
\------------------------------------------------------------------------*/
TarEntryHeader* Tar_getHeader(const TarEntryHeader* firstHeader, u32int index);

/*-------------------------------------------------------------------------
| Get number of tar files
|--------------------------------------------------------------------------
| DESCRIPTION:  Returns the number of tar files.
|
| PARAM:        'firstHeader' the first tar header
|
| RETURN        'u32int' the number of tar files
\------------------------------------------------------------------------*/
u32int Tar_getNumberOfFiles(const TarEntryHeader* firstHeader);
#endif