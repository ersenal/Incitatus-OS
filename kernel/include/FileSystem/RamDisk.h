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

/*-------------------------------------------------------------------------
| Init ram disk
|--------------------------------------------------------------------------
| DESCRIPTION:    Parse initrd tar archive and initialise the ram disk.
|
\------------------------------------------------------------------------*/
void RamDisk_init(void);

#endif