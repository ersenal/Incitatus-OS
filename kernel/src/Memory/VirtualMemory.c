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
#define PTE_INDEX(x) ((u32int) x >> 12  & 0x03FF)
#define ADDR_TO_FRAME_INDEX(addr) ((u32int) addr / FRAME_SIZE)
#define FRAME_INDEX_TO_ADDR(index) ((void*) (index * FRAME_SIZE))

/*=======================================================
    STRUCT
=========================================================*/
typedef struct PageTableEntry     PageTableEntry;
typedef struct PageTable          PageTable;
typedef struct PageDirectoryEntry PageDirectoryEntry;

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
PRIVATE PageDirectory* kernelDir;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE inline void VirtualMemory_invalidateTLB(void) {

    asm volatile(

        "movl  %cr3,%eax    \n"
        "movl  %eax,%cr3    \n"

    );

}

PRIVATE inline void VirtualMemory_invalidateTLBEntry(void* addr) {

    asm volatile("invlpg (%0)" ::"r" (addr) : "memory");

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

PRIVATE void VirtualMemory_setPDE(PageDirectoryEntry* pde, PageTable* pageTable) {

    Debug_assert(pde != NULL && pageTable != NULL);

    Memory_set(pde, 0, sizeof(PageDirectoryEntry));
    pde->frameIndex = ADDR_TO_FRAME_INDEX(pageTable);
    pde->inMemory = TRUE;
    pde->rwFlag = TRUE;

}

PRIVATE void VirtualMemory_setPTE(PageTableEntry* pte, void* physicalAddr) {

    Debug_assert(pte != NULL && (u32int) physicalAddr % FRAME_SIZE == 0);

    Memory_set(pte, 0, sizeof(PageTableEntry));
    pte->frameIndex = ADDR_TO_FRAME_INDEX(physicalAddr);
    pte->inMemory = TRUE;
    pte->rwFlag = TRUE;

}

PRIVATE void VirtualMemory_init(void) {

    /* Create the initial page directory */
    PageDirectory* dir = PhysicalMemory_allocateFrame();
    Memory_set(dir, 0, sizeof(PageDirectory));
    VirtualMemory_switchPageDir(dir);
    kernelDir = dir;

    /* Identity map first 4MB (except first 4096kb in order to catch NULLs) */
    //Console_printString("Allocating PDE: 0\n");
    PageTable* first4MB = PhysicalMemory_allocateFrame();

    for(int i = 1; i < 1024; i++) {

        PageTableEntry* pte = &first4MB->entries[i];
        VirtualMemory_setPTE(pte, FRAME_INDEX_TO_ADDR(i));

    }

    VirtualMemory_setPDE(&dir->entries[0], first4MB);
    /* End of identity map */

    /* Map directory to last virtual 4MB - recursive mapping, lets us manipulate the page directory after paging is enabled */
    dir->entries[1023].frameIndex = ADDR_TO_FRAME_INDEX(dir);
    dir->entries[1023].inMemory = TRUE;
    dir->entries[1023].rwFlag = TRUE;

    /* Register page fault handler */
    IDT_registerHandler(&VirtualMemory_pageFaultHandler, 14);

    /* Turn on paging */
    VirtualMemory_setPaging(TRUE);

}

PUBLIC PageDirectory* VirtualMemory_getKernelDir(void) {

    return kernelDir;

}

PUBLIC void VirtualMemory_switchPageDir(PageDirectory* dir) {

    Debug_assert(dir != NULL);

    /* Set page directory base register */
    u32int cr3 = CPU_getCR(3);
    CR3 reg = FORCE_CAST(cr3, CR3);
    reg.PDBR = (u32int) &dir->entries / FRAME_SIZE;
    CPU_setCR(3, FORCE_CAST(reg, u32int));

    /* Flush all TLB entries */
    VirtualMemory_invalidateTLB();
}

PUBLIC void VirtualMemory_mapKernel(Process* process) {

    Debug_assert(process != NULL);

    PageDirectory* pageDir = (PageDirectory*) process->pageDir;
    pageDir = VirtualMemory_mapPage((void*) 0xF00000, pageDir); /* Map it so that we can access it */
    PageDirectory* kDir = VirtualMemory_mapPage((void*) 0xF01000, (void*) kernelDir); /* Map it so that we can access it */

    /* Map bottom 4MB */
    PageDirectoryEntry* pde = &pageDir->entries[0];
    Memory_set(pde, 0, sizeof(PageDirectoryEntry));
    pde->frameIndex = kDir->entries[0].frameIndex;
    pde->inMemory = TRUE;
    pde->rwFlag = TRUE;

    /* Map kernel heap, first 32MB */
    for(u32int i = PDE_INDEX(KERNEL_HEAP_BASE_VADDR); i < PDE_INDEX(KERNEL_HEAP_BASE_VADDR) + 8; i++) {

        pde = &pageDir->entries[i];
        Memory_set(pde, 0, sizeof(PageDirectoryEntry));
        pde->frameIndex = kDir->entries[PDE_INDEX(KERNEL_HEAP_BASE_VADDR)].frameIndex;
        pde->inMemory = TRUE;
        pde->rwFlag = TRUE;

    }

    /* Unmap temporary mappings */
    VirtualMemory_unmapPage((void*) 0xF00000);
    VirtualMemory_unmapPage((void*) 0xF01000);

}

PUBLIC void VirtualMemory_createPageDirectory(Process* process) {

    Debug_assert(process->pageDir == NULL);

    PageDirectory* dir = (PageDirectory*) PhysicalMemory_allocateFrame();
    process->pageDir = dir;
    dir = VirtualMemory_mapPage((void*) 0xF00000, dir); /* Map it so that we can access it */
    Memory_set(dir, 0, sizeof(PageDirectory));

    /* Map directory to last virtual 4MB - recursive mapping, lets us manipulate the page directory after paging is enabled */
    dir->entries[1023].frameIndex = (ADDR_TO_FRAME_INDEX(process->pageDir));
    dir->entries[1023].inMemory = TRUE;
    dir->entries[1023].rwFlag = TRUE;
    dir->entries[1023].mode = 1; /* User mode */

    /* Set all other entries as user mode */
    for(int i = 0; i < 1023; i++)
        dir->entries[i].mode = 1;

    VirtualMemory_unmapPage((void*) 0xF00000);

}

PUBLIC void VirtualMemory_destroyPageDirectory(Process* process) {

    Debug_assert(process->pageDir != NULL);

    /* Free every page table starting at 1GB(everything except kernel which is bottom 4MB + kernel heap) */
    for(int i = PDE_INDEX(KERNEL_HEAP_TOP_VADDR); i < 1024; i++) {

        /* Map page directory so that we can access it */
        PageDirectory* dir = (PageDirectory*) VirtualMemory_mapPage(0, process->pageDir);
        PageDirectoryEntry* pde = &dir->entries[i];

        if(pde->inMemory) {

            PageTable* pageTable = (PageTable*) FRAME_INDEX_TO_ADDR(pde->frameIndex);
            Debug_assert(pageTable != NULL);
            pageTable = VirtualMemory_mapPage(0, pageTable); /* Map page table so that we can access it */

            /* Free all page table entries*/
            for(int y = 0; y < 1024; y++) {

                PageTableEntry* pte = &pageTable->entries[y];

                if(pte->inMemory) {

                    void* phys = FRAME_INDEX_TO_ADDR(pte->frameIndex);
                    Debug_assert(phys != NULL);
                    PhysicalMemory_freeFrame(phys);

                }

            }

        }

    }

    VirtualMemory_unmapPage(0);
    PhysicalMemory_freeFrame(process->pageDir);

}

PUBLIC void* VirtualMemory_mapPage(void* virtualAddr, void* physicalAddr) {

    /* Addresses should be page aligned */
    Debug_assert((u32int) virtualAddr % FRAME_SIZE == 0 && (u32int) physicalAddr % FRAME_SIZE == 0);

    PageDirectory* dir = (PageDirectory*) 0xFFFFF000; /* Get current process' page directory */
    PageDirectoryEntry* pde = &dir->entries[PDE_INDEX(virtualAddr)];

    if(!pde->inMemory) { /* Need to allocate a page table */

        //Console_printf("%s%d%c", "Allocating PDE: ", PDE_INDEX(virtualAddr), '\n');
        PageTable* pageTable = PhysicalMemory_allocateFrame();

        Debug_assert(pageTable != NULL); /* Out of physical memory */

        VirtualMemory_setPDE(pde, pageTable);
        pageTable = (PageTable*) (((u32int*) 0xFFC00000) + (0x400 * PDE_INDEX(virtualAddr)));
        Memory_set(pageTable, 0, sizeof(PageTable));

    }

    PageTable* pageTable = (PageTable*) (((u32int*) 0xFFC00000) + (0x400 * PDE_INDEX(virtualAddr)));
    PageTableEntry* pte = &pageTable->entries[PTE_INDEX(virtualAddr)];
    VirtualMemory_setPTE(pte, physicalAddr);
    VirtualMemory_invalidateTLBEntry(virtualAddr);

    return virtualAddr;

}

PUBLIC void VirtualMemory_unmapPage(void* virtualAddr) {

    /* Address should be page aligned */
    Debug_assert((u32int) virtualAddr % FRAME_SIZE == 0);

    PageDirectory* dir = (PageDirectory*) 0xFFFFF000;
    PageDirectoryEntry* pde = &dir->entries[PDE_INDEX(virtualAddr)];

    Debug_assert(pde->inMemory);

    PageTable* pageTable = (PageTable*) (((u32int*) 0xFFC00000) + (0x400 * PDE_INDEX(virtualAddr)));
    PageTableEntry* pte = &pageTable->entries[PTE_INDEX(virtualAddr)];

    Debug_assert(pte->inMemory);

    Memory_set(pte, 0, sizeof(PageTableEntry));
    VirtualMemory_invalidateTLBEntry(virtualAddr);

}

PUBLIC void* VirtualMemory_getPhysicalAddress(void* virtualAddr) {

    /* Address should be page aligned */
    Debug_assert((u32int) virtualAddr % FRAME_SIZE == 0);

    PageDirectory* dir = (PageDirectory*) 0xFFFFF000;
    PageDirectoryEntry* pde = &dir->entries[PDE_INDEX(virtualAddr)];

    Debug_assert(pde->inMemory);

    PageTable* pageTable = (PageTable*) (((u32int*) 0xFFC00000) + (0x400 * PDE_INDEX(virtualAddr)));
    PageTableEntry* pte = &pageTable->entries[PTE_INDEX(virtualAddr)];

    Debug_assert(pte->inMemory);

    return (void*) FRAME_INDEX_TO_ADDR(pte->frameIndex);
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