/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| StackPMM.c (implements PhysicalMemory)
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Stack based physical memory manager implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Memory/StackPMM.h>
#include <Lib/Stack.h>
#include <Debug.h>
#include <Memory.h>

/*=======================================================
    DEFINE
=========================================================*/
#define FRAME_SIZE 0x1000 /* 4kB, page-sized */

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE u32int totalPhysicalMemory;
PRIVATE u32int totalFrames;
PRIVATE Stack  stack;

/*=======================================================
    FUNCTION
=========================================================*/

PUBLIC void* StackPMM_allocateFrame(void) {

    if(stack.size == 0) /* out of memory */
        return NULL;

    void* frame = Stack_pop(&stack);
    //Console_printf("%s%d%c", "Physical Allocation: ", frame, '\n');
    return frame;
}

PUBLIC void StackPMM_freeFrame(void* b) {

    Debug_assert(b != NULL && (u32int) b % FRAME_SIZE == 0);
    Stack_push(&stack, b);

}

PUBLIC void StackPMM_init(MultibootInfo* mbI, MultibootHeader* mbH) {

    Debug_assert(mbI != NULL);
    Debug_assert(mbH->magic == MULTIBOOT_HEADER_MAGIC);

    MultibootMemEntry* entry = (MultibootMemEntry*) mbI->mmapAddr;

    /* calculate total physical memory */
    while((u32int) entry <  mbI->mmapAddr + mbI->mmapLength) {

        totalPhysicalMemory += entry->len;
        entry++;

    }

    totalFrames = totalPhysicalMemory / FRAME_SIZE; /* total number of frames = physical memory / 4kB */
    Stack_init(&stack, (char*) mbH->bssEndAddr, totalFrames * sizeof(void *));
    entry = (MultibootMemEntry*) mbI->mmapAddr;

    /* Push usable frames to stack */
    while((u32int) entry <  mbI->mmapAddr + mbI->mmapLength) {

        if(entry->type == MULTIBOOT_MEMORY_AVAILABLE) {

            u32int numberOfFrames = entry->len / FRAME_SIZE;

            for(u32int i = 0; i < numberOfFrames; i++) {

                void* frameAddr = (void*) (u32int) entry->addr + (i * FRAME_SIZE);

                /* Frame 0(Starting at address 0) + Kernel + PMM stack is reserved */
                if(frameAddr != NULL && (frameAddr < (void*) mbH->loadAddr || frameAddr > (void*) mbH->bssEndAddr + totalFrames * sizeof(void *)))
                    StackPMM_freeFrame(frameAddr);

            }

        }

        entry++;
    }

}

PUBLIC void StackPMM_printInfo(void) {

    Console_printf("%s%d%c%s%d%c%s%d%c",
        "Total physical memory(KB): ", totalPhysicalMemory / 1024, '\n',
        "Total frames: ", totalFrames, '\n',
        "Free frames: ", stack.size, '\n');

}