/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| String.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:   String related functions.
|
| AUTHOR:        Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef STRING_H
#define STRING_H

#include <Common.h>

bool String_startsWith(const char* string, const char* prefix);
bool String_endsWith(const char* string, const char* suffix);
char String_charAt(const char* string, int index);
u32int String_length(const char* string);
int String_charToInt(char c);
u32int String_stringToInt(const char* str, u32int base);
char* String_numberToString(int val, int base);
char* String_copy(char* restrict dest, const char* restrict src);
int String_compare(const char* left, const char* right);
u32int String_countChar(const char* s, char c);
#endif
