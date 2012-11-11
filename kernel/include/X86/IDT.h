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
    TYPE
=========================================================*/
typedef struct Regs Regs;

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
| Exception Handler
|--------------------------------------------------------------------------
| DESCRIPTION:     C-level common interrupt handler for exceptions, called
|                  from common exception handler(see IDT.s)
|
| PARAM:           "regs"   cpu registers
\------------------------------------------------------------------------*/
void IDT_handlerException(Regs regs);

/*-------------------------------------------------------------------------
| Request(IRQ) Handler
|--------------------------------------------------------------------------
| DESCRIPTION:     C-level common interrupt handler for IRQs, called from
|                  common IRQ handler(see IDT.s)
|
| PARAM:           "regs"   cpu registers
\------------------------------------------------------------------------*/
void IDT_handlerIRQ(Regs regs);

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
