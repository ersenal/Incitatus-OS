/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| String.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:   String related functions.
|
| AUTHOR:        Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Lib/String.h>
#include <Lib/Math.h>

PUBLIC bool String_startsWith(const char* self, const char* prefix) {

    int prefixLen = String_length(prefix);

    for(int i = 0; i < prefixLen; i++) {

        if(*(self + i) == *(prefix + i))
            continue;
        else
            return FALSE;
    }

    return TRUE;
}

PUBLIC bool String_endsWith(const char* self, const char* suffix) {

    int suffixLen = String_length(suffix);
    int selfLen = String_length(self);
    int y = 0;

    for(int i = selfLen - suffixLen; i < selfLen ; i++, y++) {

        if(*(self + i) == *(suffix + y))
            continue;
        else
            return FALSE;
    }

    return TRUE;
}

PUBLIC char String_charAt(const char* self, int index) {

    return *(self + index);

}

PUBLIC u32int String_length(const char* string) {

    u32int len = 0;
    while(*string++ != '\0') len++;
    return len;
}

PUBLIC int String_charToInt(char c) {

    return c - '0';

}

PUBLIC u32int String_stringToInt(const char* str, u32int base) {

    const char* ip;
    u32int num = 0;

    for (ip = str; (*ip >= '0' && *ip <= '9') || (base > 10 && (*ip >= 'a' && *ip <= 'f')); ip++) {

        u32int offset = (*ip > '9') ? *ip - 'a' + 10 : *ip - '0';
        num = num * base + offset;

    }

    return num;

}

PUBLIC char* String_numberToString(int val, int base) {

     static char buf[32] = {0};

     if(val == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
     }

     bool isNegative = val < 0;
     if(isNegative) val = val * -1;
     int i = 30;

     while(val && i) {

        buf[i] = "0123456789abcdef"[val % base];
        i--;
        val /= base;

     }

     if(isNegative) {
        buf[i] = '-';
        return &buf[i];
     }

     return &buf[i+1];
}

/* Borrowed from: http://git.musl-libc.org/cgit/musl/tree/src/string/strcpy.c */
PUBLIC char* String_copy(char* restrict dest, const char* restrict src) {

    const u8int* s = (u8int*) src;
    u8int* d = (u8int*) dest;
    while ((*d++ = *s++));
    return dest;

}

/* Borrowed from: http://git.musl-libc.org/cgit/musl/tree/src/string/strcmp.c */
PUBLIC int String_compare(const char* left, const char* right) {

    for (; *left==*right && *left && *right; left++, right++);
    return *(unsigned char *)left - *(unsigned char *)right;

}

PUBLIC u32int String_countChar(const char* s, char c) {

    u32int i;

    for (i = 0; s[i];  s[i] == c ? i++ : (u32int) s++);

    return i;
}
