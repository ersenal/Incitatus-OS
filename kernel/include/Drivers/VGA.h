/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| VGA.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  VGA text-mode buffer driver.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


/** VGA 80x25 16-color text mode */
#ifndef VGA_H
#define VGA_H

#include <Common.h>
#include <IO.h>

/*=======================================================
    DEFINE
=========================================================*/
#define VGA_RAM     0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

#define CRTC_SELECT 0x3D4
#define CRTC_MODIFY 0x3D5

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Put character
|--------------------------------------------------------------------------
| DESCRIPTION:     Puts a character in text buffer
|
| PARAM:           "index"  buffer offset
|                  "data"   two-byte data representing color and character
|                               higher byte is color attribute
|                               lower byte is the character itself
\------------------------------------------------------------------------*/
static inline void VGA_put(u16int index, u16int data) {

    *((u16int*) VGA_RAM + index) = data;

}

/*-------------------------------------------------------------------------
| Move cursor
|--------------------------------------------------------------------------
| DESCRIPTION:     Moves the text-mode cursor to specified offset.
|
| PARAM:           "cursorLoc"  buffer offset
\------------------------------------------------------------------------*/
static inline void VGA_moveCursor(u16int cursorLoc) {

    //select register 14
    IO_outB(CRTC_SELECT, 14);
    //modify register 14
    IO_outB(CRTC_MODIFY, cursorLoc >> 8); //send low byte

    //select register 15
    IO_outB(CRTC_SELECT, 15);
    //modify register 15
    IO_outB(CRTC_MODIFY, cursorLoc); //send high byte

}

#endif