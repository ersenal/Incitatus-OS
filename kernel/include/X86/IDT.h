/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| IDT.h
|--------------------------------------------------------------------------
|
| DESCRIPTION: Manages Interrupt Descriptor Table entries.
|
|
| AUTHOR:      Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef IDT_H
#define IDT_H

#include <Common.h>
#include <Module.h>

/*-------------------------------------------------------------------------
| Get IDT module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the IDT module.
|
| NOTES:           OS will not respond to software and hardware interrupts
|                  unless this module is loaded.
|
\------------------------------------------------------------------------*/
Module* IDT_getModule(void);
#endif
