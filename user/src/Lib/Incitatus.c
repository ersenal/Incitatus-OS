/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Incitatus.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  User-space system call library.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Lib/Incitatus.h>

static int syscall(int type, int p1, int p2, int p3, int p4, int p5) {

    int ret;
    asm volatile("int $0x80" : "=a" (ret) : "0" (type), "b" ((int)p1), "c" ((int)p2),
            "d" ((int)p3), "S" ((int)p4), "D" ((int)p5));
    return ret;

}

void puts(const char* str) {

    syscall(SYSCALL_PUTS, (int) str, 0, 0, 0, 0);

}

void putc(char c) {

    syscall(SYSCALL_PUTC, c, 0, 0, 0, 0);

}

void exit(int exitCode) {

    syscall(SYSCALL_EXIT, exitCode, 0, 0, 0, 0);

}

int spawn(const char* binary) {

    return syscall(SYSCALL_SPAWN, (int) binary, 0, 0, 0, 0);

}

FILE* fchdir(FILE* fd) {

    return (FILE*) syscall(SYSCALL_FCHDIR, (int) fd, 0, 0, 0, 0);

}

FILE* chdir(const char* dir) {

    return (FILE*) syscall(SYSCALL_CHDIR, (int) dir, 0, 0, 0, 0);

}

FILE* fparent(FILE* fd) {

    return (FILE*) syscall(SYSCALL_FPARENT, (int) fd, 0, 0, 0, 0);

}

FILE* readdir(FILE* fd, int index) {

    return (FILE*) syscall(SYSCALL_READDIR, (int) fd, index, 0, 0, 0);

}

FILE* finddir(FILE* fs, const char* childname) {

    return (FILE*) syscall(SYSCALL_FINDDIR, (int) fs, (int) childname, 0, 0, 0);

}

FILE* fgetcwd(void) {

    return (FILE*) syscall(SYSCALL_FGETCWD, 0, 0, 0, 0, 0);

}

FILE* open(const char* path, const char* mode) {

    return (FILE*) syscall(SYSCALL_OPEN, (int) path, (int) mode, 0, 0, 0);

}

void close(FILE* fd) {

    syscall(SYSCALL_CLOSE, (int) fd, 0, 0, 0, 0);

}

unsigned int read(FILE* fd, unsigned int offset, unsigned int count, char* buffer) {

    return syscall(SYSCALL_READ, (int) fd, offset, count, (int) buffer, 0);

}

unsigned int write(FILE* fd, unsigned int offset, unsigned int count, const char* buffer) {

    return syscall(SYSCALL_WRITE, (int) fd, offset, count, (int) buffer, 0);

}

FILE* mkdir(const char* pathname) {

    return (FILE*) syscall(SYSCALL_MKDIR, (int) pathname, 0, 0, 0, 0);

}

void* sbrk(int size) {

    return (void*) syscall(SYSCALL_SBRK, size, 0, 0, 0, 0);

}

void fstat(FILE* fd, struct stat* buf) {

    syscall(SYSCALL_FSTAT, (int) fd, (int) buf, 0, 0, 0);

}

char* getcwd(char* buf) {

    return (char*) syscall(SYSCALL_GETCWD, (int) buf, 0, 0, 0, 0);

}

char getch(void) {

    return syscall(SYSCALL_GETCH, 0, 0, 0, 0, 0);

}

void cls(void) {

    syscall(SYSCALL_CLS, 0, 0, 0, 0, 0);

}

void restart(void) {

    syscall(SYSCALL_RESTART, 0, 0, 0, 0, 0);

}

void color(unsigned int attr) {

    syscall(SYSCALL_SETCOLOR, attr, 0, 0, 0, 0);

}