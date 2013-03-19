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
#include <stdarg.h> /* we need var-args for printf */
#include <Debug.h>
#include <Memory.h>
#include <Process/Mutex.h>
#include <Lib/String.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module console;
PRIVATE u8int cursorX;
PRIVATE u8int cursorY;
PRIVATE u8int colorAttribute;

/*=======================================================
    FUNCTION
=========================================================*/
PRIVATE void Console_clearLine(u32int lineNo) {

    Debug_assert(lineNo < vgaHeight);

    for(int i = 0; i < vgaWidth; i++) {

        VGA_put(lineNo * vgaWidth + i, (CONSOLE_NORMAL << 0x8) + ' ');

    }
}

PRIVATE void Console_nextLine(void) {

    cursorX = 0;
    cursorY++;

}

PRIVATE void Console_previousLine(void) {

    cursorX = vgaWidth;
    cursorY--;

}

PRIVATE void Console_scrollDown(void) {

    /* move every line i to line i-1 */
    for(int i = 1; i < vgaHeight; i++) {

        char* src   = (char*) vgaRam + ((i) * vgaWidth * 2);
        char* dest  = (char*) vgaRam + ((i - 1) * vgaWidth * 2);
        Memory_copy(dest, src, vgaWidth * 2);

    }

    /* clear last row */
    cursorY = vgaHeight - 1;
    Console_clearLine(cursorY);

}

PRIVATE void Console_printInt(int integer) {

    char* s = String_numberToString(integer, 10);
    Console_printString(s);

}

PRIVATE void Console_init(void) {

    colorAttribute = CONSOLE_NORMAL;
    Console_clearScreen();

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

        case '\b':
            if(cursorX <= 0)
                Console_previousLine();

            VGA_put(cursorY * vgaWidth + (cursorX - 1), (colorAttribute << 0x8) + ' ');
            cursorX--;
            break;

        default:
            VGA_put(cursorY * vgaWidth + cursorX, (colorAttribute << 0x8) + c);
            cursorX++;

            if(cursorX >= vgaWidth)
                Console_nextLine();
            break;
    }

    if(cursorY >= vgaHeight) Console_scrollDown();
    VGA_moveCursor(cursorY * vgaWidth + cursorX);

}

PUBLIC void Console_printString(const char* str) {

    while(*str != '\0') {
        Console_printChar(*str);
        str++;
    }

}

PUBLIC void Console_clearScreen(void) {

    for(int i = 0; i < vgaHeight; i++)
        Console_clearLine(i);

    cursorX = 0;
    cursorY = 0;
    VGA_moveCursor(cursorY * vgaWidth + cursorX);
}

PUBLIC void Console_setColor(u8int attribute) {

    colorAttribute = attribute;

}

PUBLIC Module* Console_getModule(void) {

    console.moduleName = "Console Driver";
    console.numberOfDependencies = 1;
    console.dependencies[0] = MODULE_VGA;
    console.init = &Console_init;
    console.moduleID = MODULE_CONSOLE;

    return &console;

}

