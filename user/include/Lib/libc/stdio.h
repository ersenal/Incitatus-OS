#ifndef STDIO_H
#define STDIO_H

/* Console foreground, background colors */
#define BLACK       0x00
#define _BLACK      0x00
#define BLUE        0x01
#define _BLUE       0x10
#define GREEN       0x02
#define _GREEN      0x20
#define CYAN        0x03
#define _CYAN       0x30
#define RED         0x04
#define _RED        0x40
#define MAGENTA     0x05
#define _MAGENTA    0x50
#define BROWN       0x06
#define _BROWN      0x60
#define LGREY       0x07
#define _LGREY      0x70
#define DGREY       0x08
#define _DGREY      0x80
#define LBLUE       0x09
#define _LBLUE      0x90
#define LGREEN      0x0A
#define _LGREEN     0xA0
#define LCYAN       0x0B
#define _LCYAN      0xB0
#define LRED        0x0C
#define _LRED       0xC0
#define LMAGENTA    0x0D
#define _LMAGENTA   0xD0
#define YELLOW      0x0E
#define _YELLOW     0xE0
#define WHITE       0x0F
#define _WHITE      0xF0
#define INTENSE     0x08
#define _BLINK      0x80


int isspace(int c);
int isdigit(int c);
int atoi(const char *s);
long atol(const char *s);
char* itoa(int val, int base);
void printf(const char* template, ...);

#endif