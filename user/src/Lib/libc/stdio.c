/* musl as a whole is licensed under the following standard MIT license:

Copyright © 2005-2013 Rich Felker

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software. */

#include <Lib/libc/stdio.h>
#include <Lib/Incitatus.h>
#include <stdarg.h> /* we need var-args for printf */

/* Borrowed from musl libc */
int isspace(int c)
{
    return c == ' ' || (unsigned)c-'\t' < 5;
}

/* Borrowed from musl libc */
int isdigit(int c)
{
    return (unsigned)c-'0' < 10;
}

/* Borrowed from musl libc */
int atoi(const char *s)
{
    int n=0, neg=0;
    while (isspace(*s)) s++;
    switch (*s) {
    case '-': neg=1;
    case '+': s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
        n = 10*n - (*s++ - '0');
    return neg ? n : -n;
}

/* Borrowed from musl libc */
long atol(const char *s)
{
    long n=0;
    int neg=0;
    while (isspace(*s)) s++;
    switch (*s) {
    case '-': neg=1;
    case '+': s++;
    }
    /* Compute n as a negative number to avoid overflow on LONG_MIN */
    while (isdigit(*s))
        n = 10*n - (*s++ - '0');
    return neg ? n : -n;
}

char* itoa(int val, int base) {

     static char buf[32] = {0};

     if(val == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
     }

     unsigned char isNegative = val < 0;
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

void printf(const char* template, ...) {

    va_list args;
    va_start(args, template);

    while(*(template) != '\0') {

        if(*template == '%') {
            switch(*(template + 1)) {

                case 'a':
                color(va_arg(args, int));
                break;

                case 'd':
                puts(itoa(va_arg(args, int), 10));
                break;

                case 'h':
                puts(itoa(va_arg(args, int), 16));
                break;

                case 'c':
                putc(va_arg(args, int));
                break;

                case 's':
                puts(va_arg(args, char*));
                break;
            }
        }

        template++;
    }

    va_end(args);
}
