/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| RamDisk.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Tar-based ramdisk(RAM as disk drive) implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef RAMDISK_H
#define RAMDISK_H

#include <Module.h>

/*=======================================================
    DEFINE
=========================================================*/
#define RAMDISK_DEVICE_ID 0xBEEF

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Get ram disk module
|--------------------------------------------------------------------------
| DESCRIPTION:    Returns the ram disk module.
|
| PARAM:          'firstHeader' - Address of the first tar header
\------------------------------------------------------------------------*/
Module* RamDisk_getModule(u32int firstHeader);

#endif