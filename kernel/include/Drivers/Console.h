/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Console.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Text-Mode console driver.
|
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef CONSOLE_H
#define CONSOLE_H

#include <Common.h>

/** Console colors */
#define CONSOLE_ERROR   0x0C //light-red
#define CONSOLE_INFO    0x09 //light-blue
#define CONSOLE_WARN    0x0E //yellow
#define CONSOLE_NORMAL  0x07 //grey

/*-------------------------------------------------------------------------
| Print formatted string
|--------------------------------------------------------------------------
| DESCRIPTION:     Constructs a string using the specified template and
|                  variable arguments and then prints it.
|
| PARAM:          "template"    is used to construct the string
|                 "var-args"    replaces the format specifier in template
|
| PRECONDITION:   "template"    tokens which are valid:
|
|                 %a    set console color
|                 %c    ASCII char
|                 %s    string(char*)
|
|
| EXAMPLE:        Console_printf("%s%d", "The meaning of life: ", 42);
|
\------------------------------------------------------------------------*/
void Console_printf(const char* template, ...);

/*-------------------------------------------------------------------------
| Print character
|--------------------------------------------------------------------------
| DESCRIPTION:     Prints an ASCII character.
|
| PARAM:           "c"  char to print
|
| EXAMPLE:         Console_printChar('A');
|
\------------------------------------------------------------------------*/
void Console_printChar(u8int c);

/*-------------------------------------------------------------------------
| Print string
|--------------------------------------------------------------------------
| DESCRIPTION:     Prints a null terminated string.
|
| PARAM:           "str" null terminated string pointer
|
| PRECONDITION:    "str" needs to be null terminated
|
| EXAMPLE:         Console_printString("Hello World!");
|
\------------------------------------------------------------------------*/
void Console_printString(const char* str);

/*-------------------------------------------------------------------------
| Clear screen
|--------------------------------------------------------------------------
| DESCRIPTION:     Removes all characters from console screen.
|
\------------------------------------------------------------------------*/
void Console_clearScreen(void);

/*-------------------------------------------------------------------------
| Set print color
|--------------------------------------------------------------------------
| DESCRIPTION:     Sets the background and foreground colors of next char
|                  to be printed.
|
| PARAM:           "colorAttribute"   color to set
|
| EXAMPLE:         Console_setColor(CONSOLE_ERROR);
|
\------------------------------------------------------------------------*/
void Console_setColor(u8int colorAttribute);

/*-------------------------------------------------------------------------
| Initialise console driver
|--------------------------------------------------------------------------
| DESCRIPTION:     Sets the text-mode console driver.
|
| NOTES:           This function needs to be called before any printing
|                  attempt.
\------------------------------------------------------------------------*/
void Console_init(void);
#endif
