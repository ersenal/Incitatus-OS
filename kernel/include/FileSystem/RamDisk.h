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
|               For more info on Tar:
|                   http://en.wikipedia.org/wiki/Tar_file_format
|
|               With thanks to Brendan Long:
|                   http://stackoverflow.com/questions/2505042/how-to-parse-a-tar-file-in-c
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef RAMDISK_H
#define RAMDISK_H

#include <Module.h>

/*-------------------------------------------------------------------------
| Get ram disk module
|--------------------------------------------------------------------------
| DESCRIPTION:    Returns the ram disk module.
|
| PARAM:          'firstHeader' - Address of the first tar header
\------------------------------------------------------------------------*/
Module* RamDisk_getModule(u32int firstHeader);

#endif