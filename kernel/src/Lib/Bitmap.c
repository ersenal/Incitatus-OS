/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Bitmap.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Bit array data structure implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Lib/Bitmap.h>
#include <Debug.h>

#define CHECK_INDEX Debug_assert((index / 8) < self->length);

PUBLIC void Bitmap_setBit(Bitmap* self, u32int index) {

    CHECK_INDEX

    char* byte = self->start + (index / 8);
    *byte |= (1 << (index % 8));

}

PUBLIC void Bitmap_clearBit(Bitmap* self, u32int index) {

    CHECK_INDEX

    char* byte = self->start + (index / 8);
    *byte &= ~(1 << (index % 8));

}

PUBLIC bool Bitmap_isSet(Bitmap* self, u32int index) {

    CHECK_INDEX

    char* byte = self->start + (index / 8);
    return *byte & (1 << (index % 8));

}

PUBLIC void Bitmap_toggle(Bitmap* self, u32int index) {

    CHECK_INDEX

    if(Bitmap_isSet(self, index))
        Bitmap_clearBit(self, index);
    else
        Bitmap_setBit(self, index);

}