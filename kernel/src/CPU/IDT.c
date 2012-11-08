/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| IDT.c
|--------------------------------------------------------------------------
|
| DESCRIPTION: Initialises and stores Interrupt Descriptor Table entries.
|
|
| AUTHOR:      Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <CPU/IDT.h>
#include <Memory.h>

/*=======================================================
    DEFINE
=========================================================*/

/* Maximum number of interrupts
 *   0  to 31  = exceptions and non-maskable interrupts
 *   32 to 47  = maskable interrupts
 *   48 to 255 = software interrupts
 */
#define NUMBER_OF_INTERRUPTS 256

/* IDT gate types */
#define GATE_INTERRUPT 0b11110;
#define GATE_TRAP      0b01110;
#define GATE_TASK      0b00101;

 /* IDT privilege levels */
#define KERNEL_MODE  0
#define USER_MODE    3

/*=======================================================
    STRUCT
=========================================================*/
typedef struct IDTEntry IDTEntry;
typedef struct IDTPointer IDTPointer;

/* 8-bytes long IDT entry */
struct IDTEntry {

    /* Bits 0-15 of interrupt handler address */
    u16int handlerAddrLow   : 16;

    /* Code segment selector */
    u16int segment          : 16;

    /* Reserved */
    u8int                   :  6;

    /* Set as 0 */
    u8int  always0          :  2;

    /* Defines gate type
     *  Interrupt gate = [11110]
     *  Trap gate      = [01110]
     *  Task gate      = [00101]
     */
    u8int  gateType         :  5;

    /* Defines IDT entry privilege level.
     * If this value is greater than or equal to current
     * protection ring, the handler is executed.
     *
     * We only use 0 and 3(as do most operating systems)
     * 0 = Kernel
     * 1 and 2 = Mostly device drivers
     * 3 = User
     */
    u8int  privilege        :  2;

    /* Set 0 for unused interrupts */
    u8int  present          :  1;

    /* Bits 16-31 of interrupt handler address */
    u16int handlerAddrHigh  : 16;

} __attribute__((packed));

/* 6-bytes long IDT pointer */
struct IDTPointer {

    /* Length of IDT in bytes */
    u16int limit;

    /* Points to first IDT entry */
    IDTEntry* firstEntry;

} __attribute__((packed));

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module     idtModule;
PRIVATE IDTEntry   idtEntries[NUMBER_OF_INTERRUPTS];
PRIVATE IDTPointer idtPointer;

/*=======================================================
    FUNCTION
=========================================================*/
PRIVATE void IDT_flush(void) {

    /* Load IDT pointer */
    asm volatile ("lidt %0" : : "m" (idtPointer));

}

PRIVATE void IDT_init(void) {

    idtPointer.limit = sizeof(IDTEntry) * NUMBER_OF_INTERRUPTS - 1;
    idtPointer.firstEntry = (IDTEntry*) &idtEntries;

    /* Initially all descriptor entries are null */
    Memory_set(&idtEntries, 0, sizeof(IDTEntry) * NUMBER_OF_INTERRUPTS);

    /* Let CPU know about the IDT entries */
    IDT_flush();

}

PUBLIC Module* IDT_getModule(void) {

    idtModule.init = &IDT_init;
    idtModule.moduleName = "Interrupt Descriptor Table";
    idtModule.moduleID = MODULE_IDT;
    idtModule.numberOfDependencies = 1;
    idtModule.dependencies[0] = MODULE_GDT;

    return &idtModule;
}
