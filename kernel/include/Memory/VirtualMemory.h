/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| VirtualMemory.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Platform dependent X86 virtual memory manager, initialiser.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef VMM_H
#define VMM_H

#include <Module.h>

/*=======================================================
    TYPE
=========================================================*/
typedef struct PageDirectory PageDirectory;

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Get VMM module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns virtual memory manager module.
|
\------------------------------------------------------------------------*/
Module* VirtualMemory_getModule(void);

/*-------------------------------------------------------------------------
| Map virtual address
|--------------------------------------------------------------------------
| DESCRIPTION:     Maps a virtual address to a physical address
|
| PARAM:           "virtualAddr"   4KB aligned virtual address
|                  "physicalAddr"  4KB aligned physical address
\------------------------------------------------------------------------*/
void VirtualMemory_mapPage(void* virtualAddr, void* physicalAddr);

/*-------------------------------------------------------------------------
| Unmap virtual address
|--------------------------------------------------------------------------
| DESCRIPTION:     Unmaps a virtual address
|
| PARAM:           "virtualAddr"   4KB aligned virtual address
|
\------------------------------------------------------------------------*/
void VirtualMemory_unmapPage(void* virtualAddr);

/*-------------------------------------------------------------------------
| Get physical address
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the physical address of a mapped virtual address.
|
| PARAM:           "virtualAddr"   4KB aligned virtual address
|
\------------------------------------------------------------------------*/
void* VirtualMemory_getPhysicalAddress(void* virtualAddr);
#endif