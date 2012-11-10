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

typedef struct Regs Regs;

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
