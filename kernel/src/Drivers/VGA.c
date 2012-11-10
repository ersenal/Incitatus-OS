/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| VGA.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  VGA text-mode buffer driver.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Drivers/VGA.h>

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module vga;

/*=======================================================
    PUBLIC DATA
=========================================================*/
PUBLIC char*  vgaRam;
PUBLIC u8int  vgaHeight;
PUBLIC u8int  vgaWidth;

/*=======================================================
    FUNCTION
=========================================================*/
PRIVATE void VGA_init(void) {

    vgaRam    = (char*) 0xB8000;
    vgaWidth  = 80;
    vgaHeight = 25;

}

PUBLIC Module* VGA_getModule(void) {

    vga.moduleName = "VGA Buffer Driver";
    vga.moduleID = MODULE_VGA;
    vga.numberOfDependencies = 0;
    vga.init = &VGA_init;

    return &vga;

}