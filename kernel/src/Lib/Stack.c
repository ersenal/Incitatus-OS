/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Stack.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Stack data structure implementation.
|                   - Stores pointers(4 bytes in 32-bit)
|                   - Grows upwards
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Lib/Stack.h>
#include <Debug.h>

PUBLIC void  Stack_push(Stack* self, void* item) {

    Debug_assert(self->size * sizeof(void*) < self->length);

    *(self->start + (self->size * sizeof(void*))) = item;
    self->size++;

}

PUBLIC void* Stack_pop(Stack* self) {

    Debug_assert(self->size > 0);

    void* item = *(self->start + ((self->size - 1) * sizeof(void*)));
    self->size--;
    return item;

}

PUBLIC void* Stack_peek(Stack* self) {

    return *(self->start + (self->size * sizeof(void*)));

}

PUBLIC void  Stack_init(Stack* self, void* start, u32int length) {

    self->start = start;
    self->length = length;
    self->size = 0;

};