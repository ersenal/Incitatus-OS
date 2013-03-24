/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| BitmapPMM.c (implements PhysicalMemory)
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Bitmap based physical memory manager implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Memory/BitmapPMM.h>
#include <Lib/Bitmap.h>
#include <Debug.h>
#include <Memory.h>


/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE u32int totalPhysicalMemory;
PRIVATE u32int usedFrames;
PRIVATE u32int totalFrames;

PRIVATE char* frames;
PRIVATE Bitmap bitmap;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void BitmapPMM_clearRegion(void* regionStart, u32int regionLength) {

    u32int frameIndex = (u32int) regionStart / FRAME_SIZE;

    for(u32int i = 0; i < regionLength / FRAME_SIZE; i++) {

        if(frameIndex + i < totalFrames) {
            Bitmap_clearBit(&bitmap, frameIndex + i);
            usedFrames--;
        } else
            break;

    }

}

PRIVATE void BitmapPMM_setRegion(void* regionStart, u32int regionLength) {

    u32int frameIndex = (u32int) regionStart / FRAME_SIZE;

    for(u32int i = 0; i < regionLength / FRAME_SIZE; i++) {

        if(frameIndex + i < totalFrames) {
            Bitmap_setBit(&bitmap, frameIndex + i);
            usedFrames++;
        } else
            break;
    }

}

PUBLIC void* BitmapPMM_allocateFrame(void) {

    int frameIndex = 0;

    if(usedFrames == totalFrames) /* out of memory */
        return NULL;

    for(u32int i = 0; i < totalFrames; i++)
        if(!Bitmap_isSet(&bitmap, i)) {
            frameIndex = i;
            break;
        }

    Bitmap_setBit(&bitmap, frameIndex);
    usedFrames++;

    return (void*) (frameIndex * FRAME_SIZE);
}

PUBLIC void BitmapPMM_freeFrame(void* b) {

    Debug_assert(b != NULL);

    int frameIndex = (u32int) b / FRAME_SIZE;
    Bitmap_clearBit(&bitmap, frameIndex);
    usedFrames--;

}

PUBLIC PhysicalMemoryInfo* BitmapPMM_getInfo(PhysicalMemoryInfo* buf) {

    buf->totalMemory = totalPhysicalMemory;
    buf->totalFrames = totalFrames;
    buf->freeFrames = totalFrames - usedFrames;

    return buf;

}

PUBLIC void BitmapPMM_init(void) {

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
    frames = (char*) kernelEnd; /* put frames array at the end of kernel */
    usedFrames = totalFrames;
    Memory_set(frames, 1, (totalFrames / 8) + 1);/* initially set all frames as used */
    Bitmap_init(&bitmap, frames, (totalFrames / 8) + 1);

    entry = (MultibootMemEntry*) multibootInfo->mmapAddr;

    /* clear usable regions */
    while((u32int) entry <  multibootInfo->mmapAddr + multibootInfo->mmapLength) {

        if(entry->type == MULTIBOOT_MEMORY_AVAILABLE)
            BitmapPMM_clearRegion((void*) (u32int) entry->addr, entry->len);

        entry++;
    }

    /* set kernel + frames array + 1 frame(to be safe) as reserved/used */
    u32int reserved = (kernelEnd - mbHead.loadAddr) + ((totalFrames / 8) + 1) + FRAME_SIZE;
    BitmapPMM_setRegion((void*) mbHead.loadAddr, reserved);

    BitmapPMM_allocateFrame(); /* reserve frame starting at address 0(NULL) */

}