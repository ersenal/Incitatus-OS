/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| VirtualMemory.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Platform dependent X86 virtual memory manager, initialiser.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Memory/VirtualMemory.h>
#include <Memory/PhysicalMemory.h>
#include <X86/CPU.h>
#include <X86/IDT.h>
#include <Debug.h>
#include <Memory.h>
#pragma GCC diagnostic ignored "-Wstrict-aliasing" /* ignore FORCE_CAST compiler warning */

/*=======================================================
    DEFINE
=========================================================*/
#define PDE_INDEX(x) ((u32int) x >> 22)
#define PTE_INDEX(x) ((u32int) x >> 12)

/*=======================================================
    STRUCT
=========================================================*/
typedef struct PageTableEntry     PageTableEntry;
typedef struct PageTable          PageTable;
typedef struct PageDirectoryEntry PageDirectoryEntry;
typedef struct PageDirectory      PageDirectory;

/* 4-Byte Page Table Entry */
struct PageTableEntry {

    /* Whether the page is in memory(is present) or not
        0: Not in memory
        1: In memory
    */
    u8int  inMemory     :  1;

    /* Read/Write flag
        0: Read only
        1: Read + Write
    */
    u8int  rwFlag       :  1;

    /* Page operation mode.
        0: Page is kernel mode.
        1: Page is user mode.
    */
    u8int  mode         :  1;

    /* Reserved */
    u8int               :  2;

    /* Access flag
        0: Has not been accessed.
        1: Has been accessed.
    */
    u8int  isAccessed   :  1;

    /* Is this page dirty?
        0: Has not been written to.
        1: Has been written to.
    */
    u8int  isDirty      :  1;

    /* Reserved */
    u16int              :  2;

    /* Available for use */
    u8int               :  3;

    /* Frame address */
    u32int frameIndex   : 20;

} __attribute__((packed));

struct PageTable {

    /* 4MB of virtual memory */
    PageTableEntry entries[1024];

};

/* 4-Byte Page Directory Entry */
struct PageDirectoryEntry {

    /* Whether the page table is in memory(is present) or not
        0: Not in memory
        1: In memory
    */
    u8int  inMemory         :  1;

    /* Read/Write flag
        0: Read only
        1: Read + Write
    */
    u8int  rwFlag           :  1;

    /* Page table operation mode.
        0: Page table is kernel mode.
        1: Page table is user mode.
    */
    u8int  mode             :  1;

    /* Write-through flag
        0: Write back cache is enabled.
        1: Write through cache is enabled.
    */
    u8int  isWriteThrough   :  1;

    /* Is cache disabled?
        0: Cache disabled
        1: Cache enabled
    */
    u8int  isCached         :  1;

    /* Access flag
        0: Has not been accessed.
        1: Has been accessed.
    */
    u8int  isAccessed       :  1;

    /* Reserved */
    u8int                   :  1;

    /* Page Size
        0: 4KB
        1: 4MB
    */
    u8int  pageSize         :  1;

    u8int  globalPage       :  1;

    /* Available for use */
    u8int                   :  3;

    /* Page table frame address */
    u32int frameIndex       : 20;

} __attribute__((packed));

struct PageDirectory {

    /* 4GB of virtual memory */
    PageDirectoryEntry entries[1024];

};

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module vmmModule;
PRIVATE PageDirectory* currentDir;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE inline void VirtualMemory_invalidateTLB(void) {

    asm volatile(

        "movl  %cr3,%eax    \n"
        "movl  %eax,%cr3    \n"

    );

}

PRIVATE void VirtualMemory_setPaging(bool state) {

    u32int cr0 = CPU_getCR(0);
    CR0 reg = FORCE_CAST(cr0, CR0);
    reg.PG = state;
    CPU_setCR(0, FORCE_CAST(reg, u32int));

}

PRIVATE void VirtualMemory_pageFaultHandler(void) {

    Sys_panic("Page fault!");

}

PRIVATE void VirtualMemory_switchPageDir(PageDirectory* dir) {

    Debug_assert(dir != NULL);

    currentDir = dir;

    /* Set page directory base register */
    u32int cr3 = CPU_getCR(3);
    CR3 reg = FORCE_CAST(cr3, CR3);
    reg.PDBR = (u32int) &currentDir->entries / FRAME_SIZE;
    CPU_setCR(3, FORCE_CAST(reg, u32int));

    VirtualMemory_invalidateTLB();
}

PRIVATE void VirtualMemory_init(void) {

    /* Create the initial page directory */
    PageDirectory* dir = PhysicalMemory_allocateFrame();
    Memory_set(dir, 0, sizeof(PageDirectory));
    VirtualMemory_switchPageDir(dir);

    /* Identity map first 4MB (except first 4096kb in order to catch NULLs) */
    PageTable* table = PhysicalMemory_allocateFrame();
    Memory_set(table, 0, sizeof(PageTable));

    for (int i = 1; i < 1024; i++) {

        table->entries[i].inMemory = TRUE;
        table->entries[i].rwFlag = TRUE;
        table->entries[i].frameIndex = i;

    }

    PageDirectoryEntry* pde = &currentDir->entries[0];
    pde->inMemory = TRUE;
    pde->rwFlag = TRUE;
    pde->frameIndex = (u32int) table / FRAME_SIZE;
    /* End of identity map */

    currentDir->entries[1023] = currentDir->entries[0];

    IDT_registerHandler(&VirtualMemory_pageFaultHandler, 14);
    VirtualMemory_setPaging(TRUE); /* Turn on paging */

}

PUBLIC void VirtualMemory_allocatePDE(void* virtualAddr) {

    /* Address needs to be 4MB aligned */
    Debug_assert((u32int) virtualAddr % 0x400000 == 0);

    PageDirectoryEntry* pde = &currentDir->entries[PDE_INDEX(virtualAddr)];
    Console_printf("%d", pde->frameIndex);

    if(pde->inMemory) /* Need to free if already in memory */
        VirtualMemory_deallocatePDE(virtualAddr);

    /* Allocate a page table */
    PageTable* p = PhysicalMemory_allocateFrame();
    Memory_set(p, 0, sizeof(PageTable));
    pde->inMemory = TRUE;
    pde->rwFlag = TRUE;
    pde->frameIndex = (u32int) p / FRAME_SIZE;

    /* Allocate a frame for each PTE */
    for(int i = 0; i < 1024; i++) {

        p->entries[i].frameIndex = (u32int) PhysicalMemory_allocateFrame() / FRAME_SIZE;
        p->entries[i].inMemory = TRUE;
        p->entries[i].rwFlag = TRUE;

    }

    VirtualMemory_invalidateTLB();
}

PUBLIC void VirtualMemory_deallocatePDE(void* virtualAddr) {

    /* Address needs to be 4MB aligned */
    Debug_assert((u32int) virtualAddr % 0x400000 == 0);

    PageDirectoryEntry* pde = &currentDir->entries[PDE_INDEX(virtualAddr)];

    /* PDE should already be in memory */
    Debug_assert(pde->inMemory);

    PageTable* pageTable = (PageTable*) (pde->frameIndex * FRAME_SIZE);

    /* Free physical frames */
    for(int i = 0; i < 1024; i++)
        PhysicalMemory_freeFrame((void*) (pageTable->entries[i].frameIndex * FRAME_SIZE));

    /* Free page table */
    PhysicalMemory_freeFrame(pageTable);

    pde->inMemory = FALSE;
    pde->rwFlag = FALSE;
    pde->frameIndex = 0;

    VirtualMemory_invalidateTLB();
}

PUBLIC Module* VirtualMemory_getModule(void) {

    vmmModule.moduleName = "Virtual Memory Manager";
    vmmModule.moduleID = MODULE_VMM;
    vmmModule.init = &VirtualMemory_init;
    vmmModule.numberOfDependencies = 2;
    vmmModule.dependencies[0] = MODULE_IDT;
    vmmModule.dependencies[1] = MODULE_PMM;

    return &vmmModule;

}