/* Multiboot.h - Multiboot header file. */

/* Copyright (C) 1999,2003,2007,2008,2009  Free Software Foundation, Inc.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to
*  deal in the Software without restriction, including without limitation the
*  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
*  sell copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
*  DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
*  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#ifndef MULTI_H
#define MULTI_H

#include <Common.h>

/* The magic field should contain this. */
#define MULTIBOOT_HEADER_MAGIC                  0x1BADB002

/* This should be in %eax. */
#define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002

typedef struct MultibootHeader   MultibootHeader;
typedef struct MultibootInfo     MultibootInfo;
typedef struct MultibootMemEntry MultibootMemEntry;
typedef struct MultibootAout     MultibootAout;
typedef struct MultibootELF      MultibootELF;

struct MultibootMemEntry
{

    u32int size;
    u64int addr;
    u64int len;
    #define MULTIBOOT_MEMORY_AVAILABLE              1
    #define MULTIBOOT_MEMORY_RESERVED               2
    u32int type;

} __attribute__((packed));

/* The symbol table for a.out. */
 struct MultibootAout
 {

    u32int tabsize;
    u32int strsize;
    u32int addr;
    u32int reserved;

 } __attribute__((packed));


 /* The section header table for ELF. */
 struct MultibootELF
 {

    u32int num;
    u32int size;
    u32int addr;
    u32int shndx;

 } __attribute__((packed));

struct MultibootInfo {

    /* Multiboot info version number */
    u32int flags;

    /* Available memory from BIOS */
    u32int memLower;
    u32int memUpper;

    /* "root" partition */
    u32int bootDevice;

    /* Kernel command line */
    u32int cmdline;

    /* Boot-Module list */
    u32int modsCount;
    u32int modsAddr;

    union
    {
     MultibootAout aoutSym;
     MultibootELF elfSec;
    } u;

    /* Memory Mapping buffer */
    u32int mmapLength;
    u32int mmapAddr;

    /* Drive Info buffer */
    u32int drivesLength;
    u32int drivesAddr;

    /* ROM configuration table */
    u32int configTable;

    /* Boot Loader Name */
    u32int bootLoaderName;

    /* APM table */
    u32int apmTable;

    /* Video */
    u32int vbeControlInfo;
    u32int vbeModeInfo;
    u16int vbeMode;
    u16int vbeInterfaceSeg;
    u16int vbeInterfaceOff;
    u16int vbeInterfaceLen;

} __attribute__((packed));

struct MultibootHeader {

    /* Must be MULTIBOOT_MAGIC - see above. */
    u32int magic;

    /* Feature flags. */
    u32int flags;

    /* The above fields plus this one must equal 0 mod 2^32. */
    u32int checksum;

    /* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
    u32int headerAddr;
    u32int loadAddr;
    u32int bssEndAddr;
    u32int endAddr;
    u32int entryAddr;

} __attribute__((packed));

#endif
