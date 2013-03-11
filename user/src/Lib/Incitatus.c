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

void spawn(const char* binary) {

    syscall(SYSCALL_SPAWN, (int) binary, 0, 0, 0, 0);

}