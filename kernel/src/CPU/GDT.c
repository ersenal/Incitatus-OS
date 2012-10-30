/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| GDT.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Sets up Global Descriptor Table.
|
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <CPU/GDT.h>
#include <Debug.h>
#include <Memory.h>

/*=======================================================
    DEFINE
=========================================================*/
#define NUMBER_OF_ENTRIES 5

#define NULL_SEGMENT        0x00
#define KERNEL_CODE_SEGMENT 0x08
#define KERNEL_DATA_SEGMENT 0x10
#define USER_CODE_SEGMENT   0x18
#define USER_DATA_SEGMENT   0x20

/*=======================================================
    STRUCT
=========================================================*/
typedef struct GDTPointer  GDTPointer;
typedef struct GDTEntry    GDTEntry;

struct GDTEntry {

    u16int limitLow         : 16;
    u16int baseLow          : 16;
    u8int  baseMiddle       :  8;
    u8int  accessed         :  1;
    u8int  readWrite        :  1;
    u8int  growthDirection  :  1;
    u8int  executable       :  1;
    u8int  segmentType      :  1;
    u8int  privilege        :  2;
    u8int  present          :  1;
    u8int  limitHigh        :  4;
    u8int  reserved         :  2;
    u8int  size             :  1;
    u8int  granularity      :  1;
    u8int  baseHigh         :  8;

} __attribute__((packed)); /* Turn off gcc struct optimisation, we don't want any padding-bytes */

struct GDTPointer {

    u16int    limit;
    GDTEntry* firstEntry;

} __attribute__((packed));

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module     gdtModule;
PRIVATE GDTEntry   gdtEntries[NUMBER_OF_ENTRIES];
PRIVATE GDTPointer gdtPointer;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void GDT_flush(void) {

    extern void GDT_farJump(void);

    asm volatile (

        "lgdt %0              \n"
        "mov  %1,  %%ds       \n"
        "mov  %1,  %%es       \n"
        "mov  %1,  %%fs       \n"
        "mov  %1,  %%gs       \n"
        "mov  %1,  %%ss       \n"

        : : "m" (gdtPointer), "a" (KERNEL_DATA_SEGMENT) : "memory"
    );

    GDT_farJump();
}

PRIVATE void GDT_init(void) {

    /*
    ** Simulate a flat-memory model(all segments 0 to 4GB)
    */

    /* Null segment  */
    Memory_set(&gdtEntries[0], 0, sizeof(GDTEntry));

    /* Kernel mode code segment */
    gdtEntries[1].limitLow        = 0xFFFF;
    gdtEntries[1].limitHigh       = 0xF;
    gdtEntries[1].baseLow         = 0;
    gdtEntries[1].baseMiddle      = 0;
    gdtEntries[1].baseHigh        = 0;
    gdtEntries[1].present         = 1;
    gdtEntries[1].privilege       = 0;
    gdtEntries[1].segmentType     = 1;
    gdtEntries[1].executable      = 1;
    gdtEntries[1].growthDirection = 0;
    gdtEntries[1].readWrite       = 1;
    gdtEntries[1].accessed        = 0;
    gdtEntries[1].size            = 1;
    gdtEntries[1].granularity     = 1;
    gdtEntries[1].reserved        = 0;

    /* Kernel mode data segment */
    gdtEntries[2].limitLow        = 0xFFFF;
    gdtEntries[2].limitHigh       = 0xF;
    gdtEntries[2].baseLow         = 0;
    gdtEntries[2].baseMiddle      = 0;
    gdtEntries[2].baseHigh        = 0;
    gdtEntries[2].present         = 1;
    gdtEntries[2].privilege       = 0;
    gdtEntries[2].segmentType     = 1;
    gdtEntries[2].executable      = 0;
    gdtEntries[2].growthDirection = 0;
    gdtEntries[2].readWrite       = 1;
    gdtEntries[2].accessed        = 0;
    gdtEntries[2].size            = 1;
    gdtEntries[2].granularity     = 1;
    gdtEntries[2].reserved        = 0;

    /* User mode code segment */
    gdtEntries[3].limitLow        = 0xFFFF;
    gdtEntries[3].limitHigh       = 0xF;
    gdtEntries[3].baseLow         = 0;
    gdtEntries[3].baseMiddle      = 0;
    gdtEntries[3].baseHigh        = 0;
    gdtEntries[3].present         = 1;
    gdtEntries[3].privilege       = 3;
    gdtEntries[3].segmentType     = 1;
    gdtEntries[3].executable      = 1;
    gdtEntries[3].growthDirection = 0;
    gdtEntries[3].readWrite       = 1;
    gdtEntries[3].accessed        = 0;
    gdtEntries[3].size            = 1;
    gdtEntries[3].granularity     = 1;
    gdtEntries[3].reserved        = 0;

    /* User mode data segment */
    gdtEntries[4].limitLow        = 0xFFFF;
    gdtEntries[4].limitHigh       = 0xF;
    gdtEntries[4].baseLow         = 0;
    gdtEntries[4].baseMiddle      = 0;
    gdtEntries[4].baseHigh        = 0;
    gdtEntries[4].present         = 1;
    gdtEntries[4].privilege       = 3;
    gdtEntries[4].segmentType     = 1;
    gdtEntries[4].executable      = 0;
    gdtEntries[4].growthDirection = 0;
    gdtEntries[4].readWrite       = 1;
    gdtEntries[4].accessed        = 0;
    gdtEntries[4].size            = 1;
    gdtEntries[4].granularity     = 1;
    gdtEntries[4].reserved        = 0;

    /* Let CPU know about these entries */
    gdtPointer.limit = (sizeof(GDTEntry) * NUMBER_OF_ENTRIES) - 1;
    gdtPointer.firstEntry  = (GDTEntry*) &gdtEntries;
    GDT_flush();
}

PUBLIC Module* GDT_getModule(void) {

    gdtModule.init = &GDT_init;
    gdtModule.moduleName = "Global Descriptor Table";
    gdtModule.moduleID = MODULE_GDT;
    gdtModule.numberOfDependencies = 0;

    return &gdtModule;
}

