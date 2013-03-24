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
#define SYSCALL_PUTS        0
#define SYSCALL_PUTC        1
#define SYSCALL_EXIT        2
#define SYSCALL_SPAWN       3
#define SYSCALL_FCHDIR      4
#define SYSCALL_FPARENT     5
#define SYSCALL_GETCWD      6
#define SYSCALL_READDIR     7
#define SYSCALL_FGETCWD     8
#define SYSCALL_FSTAT       9
#define SYSCALL_GETCH       10
#define SYSCALL_CLS         11
#define SYSCALL_RESTART     12
#define SYSCALL_OPEN        13
#define SYSCALL_CLOSE       14
#define SYSCALL_READ        15
#define SYSCALL_WRITE       16
#define SYSCALL_MKDIR       17
#define SYSCALL_SBRK        18
#define SYSCALL_FINDDIR     19
#define SYSCALL_CHDIR       20
#define SYSCALL_SETCOLOR    21
#define SYSCALL_POWEROFF    22
#define SYSCALL_WAITPID     23

#define FILE int

/*=======================================================
    FUNCTION
=========================================================*/
void puts(const char* str);
void putc(char c);
void exit(int exitCode);
int spawn(const char* binary);
FILE* readdir(FILE* fd, int index);
FILE* finddir(FILE* fs, const char* childname);
FILE* fchdir(FILE* fd);
FILE* chdir(const char* dir);
FILE* fparent(FILE* fd);
FILE* fgetcwd(void);
void fstat(FILE* fd, struct stat* buf);
char* getcwd(char* buf);
char getch(void);
void cls(void);
void restart(void);
void poweroff(void);
FILE* open(const char* path, const char* mode);
void close(FILE* file);
unsigned int read(FILE* file, unsigned int offset, unsigned int count, char* buffer);
unsigned int write(FILE* file, unsigned int offset, unsigned int count, const char* buffer);
FILE* mkdir(const char* pathname);
void* sbrk(int size);
void color(unsigned int attr);
void waitpid(int pid);
#endif