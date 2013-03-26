#ifndef STDLIB_H
#define STDLIB_H

#define size_t unsigned int
#define NULL ((void*) 0)

/* Doug lea */
void* malloc(size_t size);
void* calloc(size_t numberOfElements, size_t elementSize);
void* realloc(void* oldmem, size_t bytes);
void free(void* ptr);

#endif