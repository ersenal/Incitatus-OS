/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Kernel.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Main C entry point for kernel, Kernel() function gets
|               called by Start.s.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


void Kernel(void) {

    char* framebuffer = (char*) 0xB8000;

    //Fill screen with 'A'
    for(int i = 0; i < 2000; i++) {


        *(framebuffer + (i * 2)) = 'A';

    }

    while(1) asm volatile("hlt");

}

