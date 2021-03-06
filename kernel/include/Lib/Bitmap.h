/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Bitmap.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Bit array data structure implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef BITMAP_H
#define BITMAP_H

#include <Common.h>

/*=======================================================
    STRUCT
=========================================================*/
typedef struct Bitmap Bitmap;

struct Bitmap {

    void*  start; /* Bitmap start address */
    u32int length; /* Length of bitmap in bytes */

};

/*=======================================================
    FUNCTION
=========================================================*/
void Bitmap_setBit(Bitmap* self, u32int index);
void Bitmap_clearBit(Bitmap* self, u32int index);
bool Bitmap_isSet(Bitmap* self, u32int index);
void Bitmap_toggle(Bitmap* self, u32int index);
void Bitmap_init(Bitmap* self, void* start, u32int length);
#endif