#ifndef STDIO_H
#define STDIO_H

int isspace(int c);
int isdigit(int c);
int atoi(const char *s);
long atol(const char *s);
char* itoa(int val, int base);
void printf(const char* template, ...);

#endif