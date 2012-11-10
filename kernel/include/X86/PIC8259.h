/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| 8259PIC.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Provides an interface for 8259A PIC
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef PIC_H
#define PIC_H

#include <Common.h>
#include <Module.h>

/*-------------------------------------------------------------------------
| Set IRQ mask
|--------------------------------------------------------------------------
| DESCRIPTION:     Masks a given IRQ.
|
| PARAM:           "irqNo"   the irq to mask
|                  "state"   0 = unmasked, 1 = masked
\------------------------------------------------------------------------*/
void PIC8259_setMask(u8int irqNo, bool state);

/*-------------------------------------------------------------------------
| End of interrupt
|--------------------------------------------------------------------------
| DESCRIPTION:     Sends an end of interrupt command to appropriate PIC
|                  (or PICs if the request originated from slave)
|
| PARAM:           "interruptNo"  the interrupt number of IRQ
\------------------------------------------------------------------------*/
void PIC8259_sendEOI(u8int interruptNo);

/*-------------------------------------------------------------------------
| Get PIC module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the PIC module.
\------------------------------------------------------------------------*/
Module* PIC8259_getModule(void);

#endif