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
    DEFINE
=========================================================*/
#define SYSCALL_PUTS 0
#define SYSCALL_PUTC 1
#define SYSCALL_EXIT 2

/*=======================================================
    FUNCTION
=========================================================*/
void puts(const char* str);
void putc(char c);
void exit(int exitCode);

#endif