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
    return frame;
}

PUBLIC void StackPMM_freeFrame(void* b) {

    Debug_assert(b != NULL && (u32int) b % FRAME_SIZE == 0);
    Stack_push(&stack, b);

}

PUBLIC void StackPMM_init(void) {

    extern MultibootHeader mbHead; /* Defined in Start.s */
    extern MultibootInfo* multibootInfo; /* Defined in Kernel.c */

    MultibootMemEntry* entry = (MultibootMemEntry*) multibootInfo->mmapAddr;
    u32int initrdEnd = *(u32int*)(multibootInfo->modsAddr + 4);
    u32int kernelEnd = mbHead.bssEndAddr;

    if(initrdEnd != 0)
        kernelEnd = initrdEnd;

    /* calculate total physical memory */
    while((u32int) entry <  multibootInfo->mmapAddr + multibootInfo->mmapLength) {

        totalPhysicalMemory += entry->len;
        entry++;

    }

    totalFrames = totalPhysicalMemory / FRAME_SIZE; /* total number of frames = physical memory / 4kB */
    Stack_init(&stack, (char*) kernelEnd, totalFrames * sizeof(void *));
    entry = (MultibootMemEntry*) multibootInfo->mmapAddr;

    /* Push usable frames to stack */
    while((u32int) entry <  multibootInfo->mmapAddr + multibootInfo->mmapLength) {

        if(entry->type == MULTIBOOT_MEMORY_AVAILABLE) {

            u32int numberOfFrames = entry->len / FRAME_SIZE;

            for(u32int i = 0; i < numberOfFrames; i++) {

                void* frameAddr = (void*) (u32int) entry->addr + (i * FRAME_SIZE);

                /* Frame 0(Starting at address 0) + Kernel + PMM stack is reserved */
                if(frameAddr != NULL && (frameAddr < (void*) mbHead.loadAddr || frameAddr > (void*) kernelEnd + totalFrames * sizeof(void *)))
                    StackPMM_freeFrame(frameAddr);

            }

        }

        entry++;
    }

}

PUBLIC PhysicalMemoryInfo* StackPMM_getInfo(PhysicalMemoryInfo* buf) {

    buf->totalMemory = totalPhysicalMemory;
    buf->totalFrames = totalFrames;
    buf->freeFrames = stack.size;

    return buf;

}