/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| ArrayList.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  ArrayList(dynamic array) data structure implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <Common.h>

/*=======================================================
    TYPE
=========================================================*/
typedef struct ArrayList ArrayList;

/*=======================================================
    FUNCTION
=========================================================*/
void*  ArrayList_get        (ArrayList* self, u32int index);
int    ArrayList_getIndex   (ArrayList* self, void* dataPointer);
bool   ArrayList_exists     (ArrayList* self, void* dataPointer);
u32int ArrayList_getSize    (ArrayList* self);
void   ArrayList_add        (ArrayList* self, void* dataPointer);
void   ArrayList_remove     (ArrayList* self, void* dataPointer);
void   ArrayList_removeAt   (ArrayList* self, u32int index);
bool   ArrayList_isEmpty    (ArrayList* self);

void       ArrayList_destroy(ArrayList* self);
ArrayList* ArrayList_new    (u32int initialLength);

#endif
