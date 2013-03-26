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
| RETURN:          'void*' mapped virtual address
\------------------------------------------------------------------------*/
void* VirtualMemory_mapPage(PageDirectory* dir, void* virtualAddr, void* physicalAddr, bool mode);

/*-------------------------------------------------------------------------
| Unmap virtual address
|--------------------------------------------------------------------------
| DESCRIPTION:     Unmaps a virtual address
|
| PARAM:           "virtualAddr"   4KB aligned virtual address
\------------------------------------------------------------------------*/
void VirtualMemory_unmapPage(PageDirectory* dir, void* virtualAddr);

/*-------------------------------------------------------------------------
| Get physical address
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the physical address of a mapped virtual address.
|
| PARAM:           "virtualAddr"   4KB aligned virtual address
|
| RETURN:         'void*' the physical address
\------------------------------------------------------------------------*/
void* VirtualMemory_getPhysicalAddress(void* virtualAddr);

/*-------------------------------------------------------------------------
| Quick map
|--------------------------------------------------------------------------
| DESCRIPTION:     Temporarily maps a virtual address to a physical address.
|                  Maps to current page directory.
|
| PARAM:           "virtualAddr"   4KB aligned virtual address
|                  "physicalAddr"  4KB aligned physical address
|
| RETURN:         'void*' the virtual address
\------------------------------------------------------------------------*/
void* VirtualMemory_quickMap(void* virtualAddr, void* physicalAddr);

/*-------------------------------------------------------------------------
| Quick unmap
|--------------------------------------------------------------------------
| DESCRIPTION:     Unmaps a virtual address from current page directory.
|
| PARAM:           "virtualAddr"   4KB aligned virtual address
\------------------------------------------------------------------------*/
void VirtualMemory_quickUnmap(void* virtualAddr);

/*-------------------------------------------------------------------------
| Get kernel directory
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the kernel's page directory
|
| RETURN:         'PageDirectory*' the kernel page directory
\------------------------------------------------------------------------*/
PageDirectory* VirtualMemory_getKernelDir(void);

/*-------------------------------------------------------------------------
| Switch page directory
|--------------------------------------------------------------------------
| DESCRIPTION:     Changes the current page directory(address space) to specified one.
|
| PARAM:           "dir"   the page directory to switch to
\------------------------------------------------------------------------*/
void VirtualMemory_switchPageDir(PageDirectory* dir);

/*-------------------------------------------------------------------------
| Map kernel
|--------------------------------------------------------------------------
| DESCRIPTION:     Maps the OS kernel to specified process' page directory
|
| PARAM:           'process' the process to map to
\------------------------------------------------------------------------*/
void VirtualMemory_mapKernel(Process* process);

/*-------------------------------------------------------------------------
| Create page directory
|--------------------------------------------------------------------------
| DESCRIPTION:     Creates a new page directory for the specified process.
|
| PARAM:           'process'   the process to create page directory for
\------------------------------------------------------------------------*/
void VirtualMemory_createPageDirectory(Process* process);

/*-------------------------------------------------------------------------
| Destroy page directory
|--------------------------------------------------------------------------
| DESCRIPTION:     Destroys(deallocates) a page directory from the specified process.
|
| PARAM:           'process'   the process to destroy page directory from
\------------------------------------------------------------------------*/
void VirtualMemory_destroyPageDirectory(Process* process);
#endif