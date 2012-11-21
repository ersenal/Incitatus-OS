/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Stack.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Stack data structure implementation.
|                   - Stores pointers(4 bytes in 32-bit)
|                   - Grows upwards
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef STACK_H
#define STACK_H

#include <Common.h>

/*=======================================================
    STRUCT
=========================================================*/
typedef struct Stack Stack;

struct Stack {

    void*   start;  /* Stack start address */
    u32int  length; /* Length of stack in bytes */
    u32int  size;   /* Number of items(void*) in stack */

};

/*=======================================================
    FUNCTION
=========================================================*/

void  Stack_push(Stack* self, void* item);
void* Stack_pop(Stack* self);
void* Stack_peek(Stack* self);
void  Stack_init(Stack* self, void* start, u32int length);
#endif