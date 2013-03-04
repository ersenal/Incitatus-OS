/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| VFS.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Virtual File System interface
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef VFS_H
#define VFS_H

#include <Common.h>

/*=======================================================
    DEFINE
=========================================================*/

#define VFS_FILE_NAME_SIZE  128

#define FILETYPE_NORMAL             0x01
#define FILETYPE_DIRECTORY          0x02
#define FILETYPE_CHAR_DEVICE        0x03
#define FILETYPE_BLOCK_DEVICE       0x04
#define FILETYPE_PIPE               0x05
#define FILETYPE_SOFT_SYMLINK       0x06
#define FILETYPE_HARD_SYMLINK       0x07
#define FILETYPE_MOUNTPOINT         0x08

/*=======================================================
    STRUCT
=========================================================*/

typedef struct VFS VFS;
typedef struct VFSNode VFSNode;
typedef struct DirEntry DirEntry;

struct VFS {

    u32int     deviceID;
    VFSNode*   rootNode;
    void       (*open)      (VFSNode* self);
    void       (*close)     (VFSNode* self);
    u32int     (*read)      (VFSNode* self, u32int offset, u32int count, char* buffer);
    u32int     (*write)     (VFSNode* self, u32int offset, u32int count, char* buffer);
    DirEntry*  (*readDir)   (VFSNode* self, u32int index);
    VFSNode*   (*findDir)   (VFSNode* self, const char* path);

};

struct VFSNode {

    char      fileName[VFS_FILE_NAME_SIZE]; /* The file name */
    u32int    index;                        /* Index of the node in a specific device*/
    u32int    permission;                   /* rwx Permission mask similar to Unix systems (for example 644, 777, etc.) */
    u32int    uid;                          /* Owner id */
    u32int    gid;                          /* Group id */
    u32int    fileType;                     /* Directory, normal, link, etc. */
    u32int    fileSize;                     /* Size of the file in bytes */
    VFS*      vfs;                          /* File system this node belongs to */
    VFSNode*  ptr;                          /* Used by mountpoints and symlinks */

};

struct DirEntry {

    char   fileName[VFS_FILE_NAME_SIZE];
    u32int index;

};

#endif