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

/*-------------------------------------------------------------------------
| Keyboard driver installation
|--------------------------------------------------------------------------
| DESCRIPTION:     Initialises PS/2 keyboard driver. Gets called while
|                  initialising the PS/2 controller.
|                  (refer to PS2Controller.c : PS2Controller_init)
|
\------------------------------------------------------------------------*/
void Keyboard_init(void);
#endif
