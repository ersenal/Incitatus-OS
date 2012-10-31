/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| IO.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Provides functions to read/write I/O ports.
|
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <IO.h>

PUBLIC void IO_outB(u16int port, u8int value) {

    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));

}

PUBLIC void IO_outW(u16int port, u16int value) {

    asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));

}

PUBLIC void IO_outL(u16int port, u32int value) {

    asm volatile ("outl %1, %0" : : "dN" (port), "a" (value));

}

PUBLIC u8int IO_inB(u16int port) {

    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;

}

PUBLIC u16int IO_inW(u16int port) {

    u16int ret;
    asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;

}

PUBLIC u32int IO_inL(u16int port) {

    u32int ret;
    asm volatile("inl %1, %0" : "=a" (ret) : "dN" (port));
    return ret;

}
