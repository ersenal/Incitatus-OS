/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Mouse.h
|--------------------------------------------------------------------------
|
| DESCRIPTION: Interrupt based PS/2 mouse driver.
|
| AUTHOR:      Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef MOUSE_H
#define MOUSE_H

/*-------------------------------------------------------------------------
| Mouse driver installation
|--------------------------------------------------------------------------
| DESCRIPTION:     Initialises PS/2 mouse driver. Gets called while
|                  initialising the PS/2 controller.
|                  (refer to PS2Controller.c : PS2Controller_init)
|
\------------------------------------------------------------------------*/
void Mouse_init(void);
#endif