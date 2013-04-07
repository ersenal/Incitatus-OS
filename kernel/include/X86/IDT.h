/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| IDT.h
|--------------------------------------------------------------------------
|
| DESCRIPTION: Initialises and stores Interrupt Descriptor Table entries.
|              Also provides C-level interrupt handlers for exceptions and
|              IRQs.
|
|
| AUTHOR:      Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef IDT_H
#define IDT_H

#include <Module.h>

/*=======================================================
    DEFINE
=========================================================*/

/* IRQ Number - Interrupt Number */
#define IRQ0   32
#define IRQ1   33
#define IRQ2   34
#define IRQ3   35
#define IRQ4   36
#define IRQ5   37
#define IRQ6   38
#define IRQ7   39
#define IRQ8   40
#define IRQ9   41
#define IRQ10  42
#define IRQ11  43
#define IRQ12  44
#define IRQ13  45
#define IRQ14  46
#define IRQ15  47

/*=======================================================
    STRUCT
=========================================================*/
typedef struct Regs Regs;

/* CPU state just before interrupt + interrupt info(intNo, errCode) */
struct Regs {

    /* Segment selectors */
    u32int gs, fs, es, ds;

    /* Pushed by pusha. */
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;

    /* Interrupt number and error code (if applicable). */
    u32int intNo, errCode;

    /* Pushed by the processor automatically. */
    u32int eip, cs, eflags, esp0, ss0;

} __attribute__((packed));

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Register high-level interrupt handler
|--------------------------------------------------------------------------
| DESCRIPTION:     Registers a function which gets called whenever
|                  interrupt "interruptNo" is raised.
|
| PARAM:           "functionAddr"   pointer to handler function
|                  "interruptNo"    interrupt to handle
\------------------------------------------------------------------------*/
void IDT_registerHandler(void* functionAddr, u8int interruptNo);

/*-------------------------------------------------------------------------
| High-level interrupt handler
|--------------------------------------------------------------------------
| DESCRIPTION:     C-level common interrupt handler, called from:
|                  IDT_commonHandler() (see IDT.s)
|
| PARAM:           "regs"   cpu registers
\------------------------------------------------------------------------*/
void IDT_interruptHandler(Regs* regs);

/*-------------------------------------------------------------------------
| Get IDT module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the IDT module.
|
| NOTES:           OS will not respond to software and hardware interrupts
|                  unless this module is loaded.
\------------------------------------------------------------------------*/
Module* IDT_getModule(void);
#endif
