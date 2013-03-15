/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| CircularFIFOBuffer.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  First input, first output circular buffer implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Lib/CircularFIFOBuffer.h>
#include <Memory/HeapMemory.h>
#include <Debug.h>

/*=======================================================
    STRUCT
=========================================================*/

struct CircularFIFOBuffer {

    char*  start;
    char*  end;
    char*  readPtr;
    char*  writePtr;
    u32int size;
    u32int count;

};

/*=======================================================
    FUNCTION
=========================================================*/

PUBLIC CircularFIFOBuffer* CircularFIFOBuffer_new(u32int size) {

    CircularFIFOBuffer* buffer = HeapMemory_calloc(1, sizeof(CircularFIFOBuffer));
    Debug_assert(buffer);

    buffer->start = HeapMemory_calloc(1, size);
    Debug_assert(buffer->start);

    buffer->readPtr  = buffer->start;
    buffer->writePtr = buffer->start;
    buffer->end      = buffer->start + size;
    buffer->size     = size;
    buffer->count    = 0;

    return buffer;

}

PUBLIC void CircularFIFOBuffer_destroy(CircularFIFOBuffer* buf) {

    HeapMemory_free(buf->start);
    HeapMemory_free(buf);

}

PUBLIC char CircularFIFOBuffer_read(CircularFIFOBuffer* buf) {

    Debug_assert(buf);

    if(buf->count == 0) /* Buffer should not be empty */
        return -1;

    char val = *buf->readPtr;
    buf->count--;
    buf->readPtr++;

    if (buf->readPtr == buf->end)
        buf->readPtr = buf->start;

    return val;
}

PUBLIC void CircularFIFOBuffer_write(CircularFIFOBuffer* buf, char val) {

    Debug_assert(buf);
    Debug_assert(buf->count != buf->size); /* Buffer should not be full */

    *buf->writePtr = val;
    buf->count++;
    buf->writePtr++;

    if (buf->writePtr == buf->end)
        buf->writePtr = buf->start;

}