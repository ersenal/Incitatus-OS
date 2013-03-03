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

typedef struct VFS VFS;

struct VFS {

    u32int rootID;
    u32int (*open)            (VFS* self, u32int id);
    u32int (*close)           (VFS* self, u32int id);
    u32int (*read)            (VFS* self, u32int id, u32int offset, u32int count, void* buffer);
    u32int (*write)           (VFS* self, u32int id, u32int offset, u32int count, void* buffer);
    u32int (*walk)            (VFS* self, u32int id, u32int count, const char* path);
    u32int (*getPhysicalAddr) (VFS* self, u32int id);

};

#endif