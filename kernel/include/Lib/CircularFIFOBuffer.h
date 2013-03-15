/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| CircularFIFOBuffer.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  First input, first output circular buffer implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef FIFO_H
#define FIFO_H

#include <Common.h>

typedef struct CircularFIFOBuffer CircularFIFOBuffer;

CircularFIFOBuffer* CircularFIFOBuffer_new(u32int size);
void CircularFIFOBuffer_destroy(CircularFIFOBuffer* buf);
char CircularFIFOBuffer_read(CircularFIFOBuffer* buf);
void CircularFIFOBuffer_write (CircularFIFOBuffer* buf, char val);

#endif
