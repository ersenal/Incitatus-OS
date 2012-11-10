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
#include <Module.h>

/*=======================================================
    DEFINE
=========================================================*/
#define CRTC_SELECT 0x3D4
#define CRTC_MODIFY 0x3D5

/*=======================================================
    PUBLIC DATA
=========================================================*/
extern char*  vgaRam;
extern u8int  vgaHeight;
extern u8int  vgaWidth;

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Get VGA module
|--------------------------------------------------------------------------
| DESCRIPTION:     Returns the VGA module.
|
\------------------------------------------------------------------------*/
Module* VGA_getModule(void);

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

    *((u16int*) vgaRam + index) = data;

}

/*-------------------------------------------------------------------------
| Move cursor
|--------------------------------------------------------------------------
| DESCRIPTION:     Moves the text-mode cursor to specified offset.
|
| PARAM:           "cursorLoc"  buffer offset
\------------------------------------------------------------------------*/
static inline void VGA_moveCursor(u16int cursorLoc) {

    /* Select register 14 */
    IO_outB(CRTC_SELECT, 14);
    /* Modify register 14 */
    IO_outB(CRTC_MODIFY, cursorLoc >> 8); /* Send low byte */

    /* Select register 15 */
    IO_outB(CRTC_SELECT, 15);
    /* Modify register 15 */
    IO_outB(CRTC_MODIFY, cursorLoc); /* Send high byte */

}

#endif