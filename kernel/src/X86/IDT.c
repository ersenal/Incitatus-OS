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
|              Also provides C-level interrupt handlers for exceptions and
|              IRQs.
|
|
| AUTHOR:      Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <X86/IDT.h>
#include <Memory.h>
#include <Common.h>
#include <X86/GDT.h>
#include <X86/PIC8259.h>
#include <Sys.h>
#include <Debug.h>

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
#define GATE_INTERRUPT 0b01110
#define GATE_TRAP      0b11111
#define GATE_TASK      0b10101

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

    /* Set as 0 */
    u8int  always0          :  8;

    /* Defines gate type
     *  Interrupt gate = 0b01110
     *  Trap gate      = 0b11111
     *  Task gate      = 0b10101
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
PRIVATE void       (*handlers[NUMBER_OF_INTERRUPTS]) (Regs*); /* Interrupt handler function pointers */

/*=======================================================
    EXTERNAL
=========================================================*/

/* Low-level interrupt handlers, see IDT.s */
/* 0 to 31 - exceptions */
extern void IDT_handler0(void); /* Divide by 0 */
extern void IDT_handler1(void); /* Single step (Debugger) */
extern void IDT_handler2(void); /* Non Maskable Interrupt (NMI) Pin */
extern void IDT_handler3(void); /* Breakpoint (Debugger) */
extern void IDT_handler4(void); /* Overflow */
extern void IDT_handler5(void); /* Bounds check */
extern void IDT_handler6(void); /* Undefined OpCode instruction */
extern void IDT_handler7(void); /* No coprocessor */
extern void IDT_handler8(void); /* Double Fault */
extern void IDT_handler9(void); /* Coprocessor Segment Overrun */
extern void IDT_handler10(void); /* Invalid Task State Segment (TSS) */
extern void IDT_handler11(void); /* Segment Not Present */
extern void IDT_handler12(void); /* Stack Segment Overrun */
extern void IDT_handler13(void); /* General Protection Fault */
extern void IDT_handler14(void); /* Page Fault */
extern void IDT_handler15(void); /* Unassigned */
extern void IDT_handler16(void); /* Coprocessor error */
extern void IDT_handler17(void); /* Alignment Check */
extern void IDT_handler18(void); /* Machine Check */
/* 19 to 31 - Reserved exceptions */
extern void IDT_handler19(void);
extern void IDT_handler20(void);
extern void IDT_handler21(void);
extern void IDT_handler22(void);
extern void IDT_handler23(void);
extern void IDT_handler24(void);
extern void IDT_handler25(void);
extern void IDT_handler26(void);
extern void IDT_handler27(void);
extern void IDT_handler28(void);
extern void IDT_handler29(void);
extern void IDT_handler30(void);
extern void IDT_handler31(void);
/* 32 to 255 - Free for software use */
/* 32 to 47 - IRQ */
extern void IDT_request0(void);
extern void IDT_request1(void);
extern void IDT_request2(void);
extern void IDT_request3(void);
extern void IDT_request4(void);
extern void IDT_request5(void);
extern void IDT_request6(void);
extern void IDT_request7(void);
extern void IDT_request8(void);
extern void IDT_request9(void);
extern void IDT_request10(void);
extern void IDT_request11(void);
extern void IDT_request12(void);
extern void IDT_request13(void);
extern void IDT_request14(void);
extern void IDT_request15(void);

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void IDT_setEntry(u8int num, u32int handlerAddr, u16int segment, u8int gateType, u8int privilege, u8int present) {

    idtEntries[num].handlerAddrLow  = handlerAddr & 0xFFFF;
    idtEntries[num].handlerAddrHigh = (handlerAddr >> 16) & 0xFFFF;
    idtEntries[num].segment         = segment;
    idtEntries[num].always0         = 0;
    idtEntries[num].gateType        = gateType;
    idtEntries[num].privilege       = privilege;
    idtEntries[num].present         = present;

}

PRIVATE void IDT_initExceptionHandlers(void) {

    IDT_setEntry(0,  (u32int) IDT_handler0,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(1,  (u32int) IDT_handler1,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(2,  (u32int) IDT_handler2,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(3,  (u32int) IDT_handler3,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(4,  (u32int) IDT_handler4,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(5,  (u32int) IDT_handler5,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(6,  (u32int) IDT_handler6,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(7,  (u32int) IDT_handler7,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(8,  (u32int) IDT_handler8,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(9,  (u32int) IDT_handler9,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(10, (u32int) IDT_handler10, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(11, (u32int) IDT_handler11, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(12, (u32int) IDT_handler12, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(13, (u32int) IDT_handler13, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(14, (u32int) IDT_handler14, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(15, (u32int) IDT_handler15, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(16, (u32int) IDT_handler16, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(17, (u32int) IDT_handler17, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(18, (u32int) IDT_handler18, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(19, (u32int) IDT_handler19, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(20, (u32int) IDT_handler20, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(21, (u32int) IDT_handler21, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(22, (u32int) IDT_handler22, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(23, (u32int) IDT_handler23, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(24, (u32int) IDT_handler24, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(25, (u32int) IDT_handler25, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(26, (u32int) IDT_handler26, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(27, (u32int) IDT_handler27, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(28, (u32int) IDT_handler28, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(29, (u32int) IDT_handler29, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(30, (u32int) IDT_handler30, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(31, (u32int) IDT_handler31, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);

}

PRIVATE void IDT_initIRQHandlers(void) {

    IDT_setEntry(32, (u32int) IDT_request0,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(33, (u32int) IDT_request1,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(34, (u32int) IDT_request2,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(35, (u32int) IDT_request3,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(36, (u32int) IDT_request4,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(37, (u32int) IDT_request5,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(38, (u32int) IDT_request6,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(39, (u32int) IDT_request7,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(40, (u32int) IDT_request8,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(41, (u32int) IDT_request9,  KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(42, (u32int) IDT_request10, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(43, (u32int) IDT_request11, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(44, (u32int) IDT_request12, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(45, (u32int) IDT_request13, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(46, (u32int) IDT_request14, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);
    IDT_setEntry(47, (u32int) IDT_request15, KERNEL_CODE_SEGMENT, GATE_INTERRUPT, KERNEL_MODE, 1);

}

PRIVATE void IDT_init(void) {

    /* Initialise interrupt service routines */
    IDT_initExceptionHandlers();
    IDT_initIRQHandlers();

    /* Let CPU know about the IDT entries, load IDT pointer */
    idtPointer.limit = sizeof(IDTEntry) * NUMBER_OF_INTERRUPTS - 1;
    idtPointer.firstEntry = (IDTEntry*) &idtEntries;
    asm volatile ("lidt %0" : : "m" (idtPointer));

}

PUBLIC void IDT_registerHandler(void* functionAddr, u8int interruptNo) {

    Debug_assert(functionAddr != NULL);

    handlers[interruptNo] = functionAddr;

}

PUBLIC void IDT_handlerException(Regs* regs) {

    if(handlers[regs->intNo] != NULL) {

        (*handlers[regs->intNo]) (regs);

    } else {

        Debug_logError("%d", regs->intNo);
        Sys_panic("Unhandled Exception Interrupt!");

    }

}

PUBLIC void IDT_handlerIRQ(Regs* regs) {

    if(handlers[regs->intNo] != NULL) {

        (*handlers[regs->intNo]) (regs);

    } else{

        Debug_logError("%d", regs->intNo);
        Sys_panic("Unhandled IRQ Interrupt!");

    }

    PIC8259_sendEOI(regs->intNo);
}

PUBLIC Module* IDT_getModule(void) {

    idtModule.init = &IDT_init;
    idtModule.moduleName = "Interrupt Descriptor Table";
    idtModule.moduleID = MODULE_IDT;
    idtModule.numberOfDependencies = 2;
    idtModule.dependencies[0] = MODULE_GDT;
    idtModule.dependencies[1] = MODULE_PIC8259;

    return &idtModule;
}
