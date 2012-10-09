/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Console.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Text-Mode console driver.
|
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Drivers/Console.h>
#include <Drivers/VGA.h>
#include <stdarg.h> //we need var-args for printf
#include <Debug.h>

/*=======================================================
    DEFINE
=========================================================*/
PRIVATE u8int cursorX;
PRIVATE u8int cursorY;
PRIVATE u8int colorAttribute;

/*=======================================================
    FUNCTION
=========================================================*/
PRIVATE void Console_clearLine(u32int lineNo) {

    Debug_assert(lineNo < VGA_HEIGHT);

    for(int i = 0; i < VGA_WIDTH; i++) {

        VGA_put(lineNo * VGA_WIDTH + i, (CONSOLE_NORMAL << 0x8) + ' ');

    }
}

PRIVATE void Console_nextLine(void) {

    cursorX = 0;
    cursorY++;

}

PRIVATE char* Console_intToString(int val, u8int base) {

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

PRIVATE void Console_printInt(int integer) {

    char* s = Console_intToString(integer, 10);
    Console_printString(s);

}

PUBLIC void Console_printf(const char* template, ...) {

    va_list args;
    va_start(args, template);

    while(*(template) != '\0') {

        if(*template == '%') {
            switch(*(template + 1)) {

                case 'a':
                Console_setColor(va_arg(args, int));
                break;

                case 'd':
                Console_printInt(va_arg(args, int));
                break;

                case 'c':
                Console_printChar(va_arg(args, int));
                break;

                case 's':
                Console_printString(va_arg(args, char*));
                break;
            }
        }

        template++;
    }

    va_end(args);
}

PUBLIC void Console_printChar(u8int c) {

    switch(c) {

        case '\n':
        Console_nextLine();
        break;

        default:
        VGA_put(cursorY * VGA_WIDTH + cursorX, (colorAttribute << 0x8) + c);
        cursorX++;

        if(cursorX >= VGA_WIDTH)
            Console_nextLine();
        break;
    }

    //TODO: scroll down if(cursorY >= VGA_HEIGHT)
    VGA_moveCursor(cursorY * VGA_WIDTH + cursorX);
}

PUBLIC void Console_printString(const char* str) {

    while(*str != '\0') {
        Console_printChar(*str);
        str++;
    }

}

PUBLIC void Console_clearScreen(void) {

    for(int i = 0; i < VGA_HEIGHT; i++)
        Console_clearLine(i);

    cursorX = 0;
    cursorY = 0;
    VGA_moveCursor(cursorY * VGA_WIDTH + cursorX);
}

PUBLIC void Console_setColor(u8int attribute) {

    colorAttribute = attribute;

}

PUBLIC void Console_init(void) {

    cursorX = 0;
    cursorY = 0;
    colorAttribute = CONSOLE_NORMAL;

}

