#ifndef STRING_H
#define STRING_H

#include <Lib/libc/stdlib.h>

void *memset(void *dest, int c, size_t n);
void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
size_t strlen(const char *s);
int strcmp(const char *l, const char *r);
char *strcpy(char *restrict dest, const char *restrict src);
char *strcat(char *restrict dest, const char *restrict src);

#endif