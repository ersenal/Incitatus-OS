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


#include <X86/GDT.h>
#include <Memory.h>

/*=======================================================
    DEFINE
=========================================================*/
#define NUMBER_OF_ENTRIES 6

/* GDT segment executable types */
#define CODE_SEGMENT 1
#define DATA_SEGMENT 0

/*=======================================================
    STRUCT
=========================================================*/
typedef struct GDTPointer  GDTPointer;
typedef struct GDTEntry    GDTEntry;
typedef struct TSSEntry   TSSEntry;

/* 8-bytes long GDT entry */
struct GDTEntry {

    /* Bits 0-15 of segment limit */
    u16int limitLow         : 16;

    /* Bits 0-15 of segment base address */
    u16int baseLow          : 16;

    /* Bits 15-23 of segment base address */
    u8int  baseMiddle       :  8;

    /* Set 0 as default, CPU sets this to 1 if the segment is accessed */
    u8int  accessed         :  1;

    /* For data segments 0 = Read-Only, 1 = Read/Write.
     * For code segments 0 = Execute-Only, 1 = Read/Execute
     */
    u8int  readWrite        :  1;

    /* For data segments, describes which direction the segment will grow.
     * 0 = Grow-Up, 1 = Grow-Down
     *
     * For code segments,
     * 1 = The segment can be executed from an equal or lower privilege level.
     * 0 = Segment can only be executed from the protection ring defined in "privilege".
     */
    u8int  directionConform :  1;

    /* Defines whether the segment is executable.
     *  1 = Executable - Code segment
     *  0 = Not Executable - Data segment
     */
    u8int  executable       :  1;

    /* Defines the descriptor type.
     * 0 = System descriptor (Null segment)
     * 1 = Code/Data descriptor (All segments except Null)
     */
    u8int  segmentType      :  1;

    /*
     * Protection ring for this segment
     * We only use 0 and 3(as do most operating systems)
     * 0 = Kernel
     * 1 and 2 = Mostly device drivers
     * 3 = User
     */
    u8int  privilege        :  2;

    /* This bit should be set for all valid segment selectors */
    u8int  present          :  1;

    /* Bits 16-19 of segment limit */
    u8int  limitHigh        :  4;

    /* Reserved, should be zero */
    u8int  reserved         :  2;

    /* Is this a 16-bit or 32-bit segment?
     * 0 = 16-bit
     * 1 = 32-bit
     */
    u8int  size             :  1;

    /* Segment limit granularity
     * 0 = 1 Byte
     * 1 = 4 KB(Same as page granularity)
     */
    u8int  granularity      :  1;

    /* Bits 24-31 of segment base address */
    u8int  baseHigh         :  8;

} __attribute__((packed)); /* Turn off gcc struct optimisation, we don't want any padding-bytes */

/* 6-bytes long GDT pointer */
struct GDTPointer {

    /* Length of GDT in bytes*/
    u16int    limit;

    /* Points to first GDT entry */
    GDTEntry* firstEntry;

} __attribute__((packed));

/* Task state segment */
struct TSSEntry {

    u32int      : 32;
    u32int esp0 : 32;
    u32int ss0  : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;
    u32int      : 32;

} __attribute__((packed));

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module     gdtModule;
PRIVATE GDTEntry   gdtEntries[NUMBER_OF_ENTRIES];
PRIVATE GDTPointer gdtPointer;
PRIVATE TSSEntry   tss;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void GDT_flushTSS(void) {

    asm volatile("ltr %w0" : : "r" (TSS_SEGMENT | 3)); /* Add 3 so that it has an RPL of 3 (User ring) */

}

PRIVATE void GDT_setTSSEntry(u32int base, u32int limit, u8int access, u8int granularity) {

    struct GDTENTRY {

        u16int limitLow;
        u16int baseLow;
        u8int baseMiddle;
        u8int access;
        u8int granularity;
        u8int base_high;

    } __attribute__((packed));

    struct GDTENTRY* ent = (struct GDTENTRY*) &gdtEntries[5];

    ent->baseLow     = (base & 0xFFFF);
    ent->baseMiddle  = (base >> 16) & 0xFF;
    ent->base_high    = (base >> 24) & 0xFF;
    ent->limitLow    = (limit & 0xFFFF);
    ent->granularity  = ( (limit >> 16) & 0x0F );
    ent->granularity  |= (granularity & 0xF0);
    ent->access       = access;
}

PRIVATE void GDT_flush(void) {

    asm volatile (

        /* Load GDT pointer */
        "lgdt %1              \n"

        /* Set segment registers */
        "mov  %2,  %%ds       \n"
        "mov  %2,  %%es       \n"
        "mov  %2,  %%fs       \n"
        "mov  %2,  %%gs       \n"
        "mov  %2,  %%ss       \n"

        /* Far jump, set code segment register(CS) and instruction pointer register(EIP) */
        "ljmp %0,  $dummy     \n"
        "dummy:               \n"

        : : "i" (KERNEL_CODE_SEGMENT),
            "m" (gdtPointer),
            "a" (KERNEL_DATA_SEGMENT) : "memory"
    );

}

PRIVATE void GDT_init(void) {

    Debug_logInfo("%s%s", "Initialising ", gdtModule.moduleName);

    /*
    ** Simulate a flat-memory model(all segments 0 to 4GB)
    */

    /* Kernel mode code segment */
    gdtEntries[1].limitLow         = 0xFFFF;
    gdtEntries[1].limitHigh        = 0xF;
    gdtEntries[1].baseLow          = 0;
    gdtEntries[1].baseMiddle       = 0;
    gdtEntries[1].baseHigh         = 0;
    gdtEntries[1].present          = 1;
    gdtEntries[1].privilege        = KERNEL_MODE;
    gdtEntries[1].segmentType      = 1;
    gdtEntries[1].executable       = CODE_SEGMENT;
    gdtEntries[1].directionConform = 0;
    gdtEntries[1].readWrite        = 1;
    gdtEntries[1].accessed         = 0;
    gdtEntries[1].size             = 1;
    gdtEntries[1].granularity      = 1;
    gdtEntries[1].reserved         = 0;

    /* Kernel mode data segment */
    gdtEntries[2].limitLow         = 0xFFFF;
    gdtEntries[2].limitHigh        = 0xF;
    gdtEntries[2].baseLow          = 0;
    gdtEntries[2].baseMiddle       = 0;
    gdtEntries[2].baseHigh         = 0;
    gdtEntries[2].present          = 1;
    gdtEntries[2].privilege        = KERNEL_MODE;
    gdtEntries[2].segmentType      = 1;
    gdtEntries[2].executable       = DATA_SEGMENT;
    gdtEntries[2].directionConform = 0;
    gdtEntries[2].readWrite        = 1;
    gdtEntries[2].accessed         = 0;
    gdtEntries[2].size             = 1;
    gdtEntries[2].granularity      = 1;
    gdtEntries[2].reserved         = 0;

    /* User mode code segment */
    gdtEntries[3].limitLow         = 0xFFFF;
    gdtEntries[3].limitHigh        = 0xF;
    gdtEntries[3].baseLow          = 0;
    gdtEntries[3].baseMiddle       = 0;
    gdtEntries[3].baseHigh         = 0;
    gdtEntries[3].present          = 1;
    gdtEntries[3].privilege        = USER_MODE;
    gdtEntries[3].segmentType      = 1;
    gdtEntries[3].executable       = CODE_SEGMENT;
    gdtEntries[3].directionConform = 0;
    gdtEntries[3].readWrite        = 1;
    gdtEntries[3].accessed         = 0;
    gdtEntries[3].size             = 1;
    gdtEntries[3].granularity      = 1;
    gdtEntries[3].reserved         = 0;

    /* User mode data segment */
    gdtEntries[4].limitLow         = 0xFFFF;
    gdtEntries[4].limitHigh        = 0xF;
    gdtEntries[4].baseLow          = 0;
    gdtEntries[4].baseMiddle       = 0;
    gdtEntries[4].baseHigh         = 0;
    gdtEntries[4].present          = 1;
    gdtEntries[4].privilege        = USER_MODE;
    gdtEntries[4].segmentType      = 1;
    gdtEntries[4].executable       = DATA_SEGMENT;
    gdtEntries[4].directionConform = 0;
    gdtEntries[4].readWrite        = 1;
    gdtEntries[4].accessed         = 0;
    gdtEntries[4].size             = 1;
    gdtEntries[4].granularity      = 1;
    gdtEntries[4].reserved         = 0;

    /* Set TSS */
    GDT_setTSSEntry((u32int)&tss, (u32int) &tss + 64, 0x89, 0x00);
    GDT_setTSS(KERNEL_DATA_SEGMENT, 0);

    /* Let CPU know about these entries */
    gdtPointer.limit = (sizeof(GDTEntry) * NUMBER_OF_ENTRIES) - 1;
    gdtPointer.firstEntry  = (GDTEntry*) &gdtEntries;
    GDT_flush();
    GDT_flushTSS();
}

PUBLIC void GDT_setTSS(u32int dataSegment, u32int esp0) {

    tss.ss0 = dataSegment;
    tss.esp0 = esp0;

}

PUBLIC Module* GDT_getModule(void) {

    if(!gdtModule.isLoaded) {

        gdtModule.init = &GDT_init;
        gdtModule.moduleName = "Global Descriptor Table";
        gdtModule.moduleID = MODULE_GDT;

    }

    return &gdtModule;
}

