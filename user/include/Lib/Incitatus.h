/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Incitatus.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  User-space system call library.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef INCITATUS_H
#define INCITATUS_H

/*=======================================================
    STRUCT
=========================================================*/

struct stat {

    char            fileName[128]; /* The file name */
    unsigned int    index;         /* Index of the node in a specific device*/
    unsigned int    permission;    /* rwx Permission mask similar to Unix systems (for example 644, 777, etc.) */
    unsigned int    mode;          /* 0 = file is not open, 1 = read mode, 2 = write mode */
    unsigned int    uid;           /* Owner id */
    unsigned int    gid;           /* Group id */
    unsigned int    fileType;      /* Directory, normal, link, etc. */
    unsigned int    fileSize;      /* Size of the file in bytes */
    void*           vfs;           /* File system this node belongs to */
    void**          ptr;           /* Used by mountpoints and symlinks */

};


/*=======================================================
    DEFINE
=========================================================*/
#define SYSCALL_PUTS    0
#define SYSCALL_PUTC    1
#define SYSCALL_EXIT    2
#define SYSCALL_SPAWN   3
#define SYSCALL_FCHDIR  4
#define SYSCALL_FPARENT 5
#define SYSCALL_GETCWD  6
#define SYSCALL_FINDDIR 7
#define SYSCALL_FGETCWD 8
#define SYSCALL_FSTAT   9

#define FILE int

/*=======================================================
    FUNCTION
=========================================================*/
void puts(const char* str);
void putc(char c);
void exit(int exitCode);
void spawn(const char* binary);
FILE readdir(FILE fd, int index);
FILE fchdir(FILE fd);
FILE fparent(FILE fd);
FILE fgetcwd(void);
void fstat(FILE fd, struct stat* buf);
char* getcwd(char* buf);
#endif