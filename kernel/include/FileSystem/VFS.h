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
#include <Module.h>

/*=======================================================
    DEFINE
=========================================================*/
#define FILE_MODE_NOT_OPEN  0
#define FILE_MODE_READ      1
#define FILE_MODE_WRITE     2

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

struct VFS {

    u32int     deviceID;
    VFSNode*   rootNode;
    void       (*open)      (VFSNode* self);
    void       (*close)     (VFSNode* self);
    u32int     (*read)      (VFSNode* self, u32int offset, u32int count, char* buffer);
    u32int     (*write)     (VFSNode* self, u32int offset, u32int count, const char* buffer);
    VFSNode*   (*readDir)   (VFSNode* self, u32int index);
    VFSNode*   (*findDir)   (VFSNode* self, const char* path);

};

struct VFSNode {

    char      fileName[VFS_FILE_NAME_SIZE]; /* The file name */
    u32int    index;                        /* Index of the node in a specific device*/
    u32int    permission;                   /* rwx Permission mask similar to Unix systems (for example 644, 777, etc.) */
    u32int    mode;                         /* 0 = file is not open, 1 = read mode, 2 = write mode */
    u32int    uid;                          /* Owner id */
    u32int    gid;                          /* Group id */
    u32int    fileType;                     /* Directory, normal, link, etc. */
    u32int    fileSize;                     /* Size of the file in bytes */
    VFS*      vfs;                          /* File system this node belongs to */
    VFSNode*  ptr;                          /* Used by mountpoints and symlinks */

};

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Open file
|--------------------------------------------------------------------------
| DESCRIPTION:  Opens a file and returns a file descriptor(node pointer).
|
| PARAM:    'filename' name of the file to be opened
|
|           'mode'     'r' Open file for input operations. The file must exist.
|
|                      'w' Create an empty file for output operations.
|                          If a file with the same name already exists, its
|                          contents are discarded and the file is treated as a new empty file.
|
| RETURN:  'VFSNode*' the file node
\------------------------------------------------------------------------*/
VFSNode* VFS_openFile(const char* filename, const char* mode);

/*-------------------------------------------------------------------------
| Close file
|--------------------------------------------------------------------------
| DESCRIPTION:  Closes an opened file.
|
| PARAM:        'file' - descriptor(pointer) of the file node to be closed
|
| RETURN:       'bool' - 0 if file closed successfully
\------------------------------------------------------------------------*/
bool VFS_closeFile(VFSNode* file);

/*-------------------------------------------------------------------------
| Change working directory
|--------------------------------------------------------------------------
| DESCRIPTION:  Changes the current process' working directory
|
| PARAM:        'file' - destination
|
| RETURN:       'VFSNode*' - destination file descriptor
\------------------------------------------------------------------------*/
VFSNode* VFS_changeDirectory(VFSNode* file);

/*-------------------------------------------------------------------------
| Get parent directory
|--------------------------------------------------------------------------
| DESCRIPTION:  Returns the parent directory
|
| PARAM:        'child' - the child file
|
| RETURN:       'VFSNode*' - parent directory
\------------------------------------------------------------------------*/
VFSNode* VFS_getParent(VFSNode* child);

/*-------------------------------------------------------------------------
| Read dir entry
|--------------------------------------------------------------------------
| DESCRIPTION:  Return nth child of a directory.
|
| PARAM:        'dir' - directory file
|               'index' - index of child
|
| RETURN:       'VFSNode*' - nth child file
\------------------------------------------------------------------------*/
VFSNode* VFS_readDir(VFSNode* dir, int index);

/*-------------------------------------------------------------------------
| Get current working directory, string
|--------------------------------------------------------------------------
| DESCRIPTION:  Returns the current process' working directory
|
| PARAM:        'buf' - buffer to store working directory path
|
| RETURN:       'char*' - the buffer containing the working directory.
\------------------------------------------------------------------------*/
char* VFS_getWorkingDirectoryStr(char* buf);

/*-------------------------------------------------------------------------
| Get current working directory, file descriptor
|--------------------------------------------------------------------------
| DESCRIPTION:  Returns the current process' working directory
|
| RETURN:       'VFSNode*' - working directory file descriptor
\------------------------------------------------------------------------*/
VFSNode* VFS_getWorkingDirectoryPtr(void);

/*-------------------------------------------------------------------------
| Get file stats
|--------------------------------------------------------------------------
| DESCRIPTION:  Fills the specified buffer with file information.
|
| PARAM:        'file' - file to be examined
|               'buf'  - buffer to be filled
\------------------------------------------------------------------------*/
void VFS_getFileStats(VFSNode* file, VFSNode* buf);

/*-------------------------------------------------------------------------
| Get VFS module
|--------------------------------------------------------------------------
| DESCRIPTION:    Returns the virtual file system module.
|
\------------------------------------------------------------------------*/
Module* VFS_getModule(void);

//TODO: comment
VFSNode* VFS_findDir(VFSNode* dir, const char* path);
u32int VFS_read(VFSNode* self, u32int offset, u32int count, char* buffer);
u32int VFS_write(VFSNode* self, u32int offset, u32int count, const char* buffer);
VFSNode* VFS_mkdir(const char* pathname);

#endif