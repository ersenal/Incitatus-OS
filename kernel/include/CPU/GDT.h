/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| GDT.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Sets up Global Descriptor Table.
|
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef GDT_H
#define GDT_H

#include <Module.h>

/*-------------------------------------------------------------------------
| Get GDT module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the GDT module.
|
\------------------------------------------------------------------------*/
Module* GDT_getModule(void);
#endif
