/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Keyboard.h
|--------------------------------------------------------------------------
|
| DESCRIPTION: Interrupt based PS/2 keyboard driver.
|
| AUTHOR:      Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Common.h>

/*-------------------------------------------------------------------------
| Keyboard driver installation
|--------------------------------------------------------------------------
| DESCRIPTION:     Initialises PS/2 keyboard driver. Gets called while
|                  initialising the PS/2 controller.
|                  (refer to PS2Controller.c : PS2Controller_init)
|
\------------------------------------------------------------------------*/
void Keyboard_init(void);

/*-------------------------------------------------------------------------
| Keyboard set leds
|--------------------------------------------------------------------------
| DESCRIPTION:     Sets the keyboard led status.
|
| PARAMS:          'numLock'    -\
|                  'capsLock'   -=> TRUE = Turn on, FALSE = Turn off.
|                  'scrollLock' -/
\------------------------------------------------------------------------*/
void Keyboard_setLeds(bool numLock, bool capsLock, bool scrollLock);

/*-------------------------------------------------------------------------
| Keyboard get char
|--------------------------------------------------------------------------
| DESCRIPTION:     Reads and returns a character stored inside circular key
|                  buffer.
|
| RETURN:          'char'  -1 if no character in buffer, char value if there is
|
| NOTES:           Currently polls.
\------------------------------------------------------------------------*/
char Keyboard_getChar(void);
#endif
