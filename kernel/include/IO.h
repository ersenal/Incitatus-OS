/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| IO.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Provides functions to read/write I/O ports.
|
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef IO_H
#define IO_H

#include <Common.h>

/*=======================================================
    DEFINE
=========================================================*/

/* Serial I/O ports */
#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| I/O Write
|--------------------------------------------------------------------------
| DESCRIPTION:     Writes n bytes to specified I/O port.
|
| PARAM:          "port"   port to write
|                 "value"  value to be written
|
| NOTES:           outL     n = 4
|                  outW     n = 2
|                  outB     n = 1
\------------------------------------------------------------------------*/
static inline void IO_outB(u16int port, u8int value) {

    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));

}

static inline void IO_outW(u16int port, u16int value) {

    asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));

}

static inline void IO_outL(u16int port, u32int value) {

    asm volatile ("outl %1, %0" : : "dN" (port), "a" (value));

}

/*-------------------------------------------------------------------------
| I/O Read
|--------------------------------------------------------------------------
| DESCRIPTION:     Reads n bytes from specified I/O port.
|
| PARAM:          "port" port to read
|
| RETURN:          n bytes
|
| NOTES:           inL     n = 4
|                  inW     n = 2
|                  inB     n = 1
\------------------------------------------------------------------------*/
static inline u8int IO_inB(u16int port) {

    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;

}

static inline u16int IO_inW(u16int port) {

    u16int ret;
    asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;

}

static inline u32int IO_inL(u16int port) {

    u32int ret;
    asm volatile("inl %1, %0" : "=a" (ret) : "dN" (port));
    return ret;

}

#endif
