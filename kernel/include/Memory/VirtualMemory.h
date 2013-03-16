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
#include <Process/ProcessManager.h>

/*=======================================================
    DEFINE
=========================================================*/
#define MODE_KERNEL 0
#define MODE_USER   1

#define TEMPORARY_MAP_VADDR 0xF00000

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
|                  "mode"          0 - Kernel mode, 1 - User mode
|
| RETURN:          Mapped virtual address
\------------------------------------------------------------------------*/
void* VirtualMemory_mapPage(PageDirectory* dir, void* virtualAddr, void* physicalAddr, bool mode);

/*-------------------------------------------------------------------------
| Unmap virtual address
|--------------------------------------------------------------------------
| DESCRIPTION:     Unmaps a virtual address
|
| PARAM:           "virtualAddr"   4KB aligned virtual address
|
\------------------------------------------------------------------------*/
void VirtualMemory_unmapPage(PageDirectory* dir, void* virtualAddr);

/*-------------------------------------------------------------------------
| Get physical address
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the physical address of a mapped virtual address.
|
| PARAM:           "virtualAddr"   4KB aligned virtual address
|
\------------------------------------------------------------------------*/
void* VirtualMemory_getPhysicalAddress(void* virtualAddr);

//TODO: add comments
void* VirtualMemory_quickMap(void* virtualAddr, void* physicalAddr);
void VirtualMemory_quickUnmap(void* virtualAddr);
PageDirectory* VirtualMemory_getKernelDir(void);
void VirtualMemory_switchPageDir(PageDirectory* dir);
void VirtualMemory_mapKernel(Process* process);
void VirtualMemory_createPageDirectory(Process* process);
void VirtualMemory_destroyPageDirectory(Process* process);
#endif