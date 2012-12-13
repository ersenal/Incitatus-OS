/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| ArrayList.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  ArrayList(dynamic array) data structure implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Lib/ArrayList.h>
#include <Memory/HeapMemory.h>
#include <Debug.h>

/*=======================================================
    STRUCT
=========================================================*/

struct ArrayList {

    void** arrayPointer;
    u32int usedSize;
    u32int totalSize;

};

/*=======================================================
    FUNCTION
=========================================================*/

PUBLIC void* ArrayList_get(ArrayList* self, u32int index) {

    Debug_assert(index < self->usedSize);
    return *(self->arrayPointer + index);

}

PUBLIC void ArrayList_add(ArrayList* self, void* dataPointer) {

    if(self->usedSize == self->totalSize) { /* grow array */

        self->totalSize = self->totalSize * 2;
        self->arrayPointer = (void**) HeapMemory_realloc(self->arrayPointer, self->totalSize * sizeof(void*));
        Debug_assert(self->arrayPointer);

    }

    *(self->arrayPointer + self->usedSize) = dataPointer; /* insert data pointer */
    self->usedSize++;

}

PUBLIC int ArrayList_getIndex(ArrayList* self, void* dataPointer) {

    for(u32int i = 0; i < self->usedSize; i++)
        if(*(self->arrayPointer + i) == dataPointer)
            return i;

    return -1; /* not found */

}

PUBLIC bool ArrayList_exists(ArrayList* self, void* dataPointer) {

    if(ArrayList_getIndex(self, dataPointer) == -1)
        return FALSE;
    else
        return TRUE;

}

PUBLIC u32int ArrayList_getSize(ArrayList* self) {

    return self->usedSize;

}

PUBLIC void ArrayList_remove(ArrayList* self, void* dataPointer) {

    u32int index = ArrayList_getIndex(self, dataPointer);
    ArrayList_removeAt(self, index);

}

PUBLIC void ArrayList_removeAt(ArrayList* self, u32int index) {

    Debug_assert(index < self->usedSize);

    /* shift to left */
    for(u32int i = index; i < self->usedSize - 1; i++)
         /* set the current element val to the val of element on the right */
        *(self->arrayPointer + i) = *(self->arrayPointer + (i + 1));

    self->usedSize--;
    self->totalSize = self->usedSize + 1;

    /* reallocate */
    self->arrayPointer = (void**) HeapMemory_realloc(self->arrayPointer, self->totalSize * sizeof(void*));
    Debug_assert(self->arrayPointer);

}

PUBLIC bool ArrayList_isEmpty(ArrayList* self) {

    if(self->arrayPointer != NULL && *(self->arrayPointer) != NULL)
        return TRUE;
    else
        return FALSE;

}

PUBLIC void ArrayList_destroy(ArrayList* self) {

    HeapMemory_free(self->arrayPointer);
    HeapMemory_free(self);

}

PUBLIC ArrayList* ArrayList_new(u32int initialLength) {

    /* allocate memory */
    ArrayList* self = (ArrayList*) HeapMemory_alloc(sizeof(ArrayList));
    Debug_assert(self);

    Debug_assert(initialLength > 0);
    self->arrayPointer = (void**) HeapMemory_alloc(sizeof(void*) * initialLength);
    Debug_assert(self->arrayPointer);

    /* set properties */
    self->totalSize = initialLength;
    self->usedSize = 0;

    return self;
}
