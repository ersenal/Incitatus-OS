/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Sys.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Provides important system functions.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Sys.h>
#include <Drivers/Console.h>
#include <Common.h>
#include <IO.h>

/*=======================================================
    DEFINE
=========================================================*/
#define SYS_C_RESET 0xFE
#define PS2_STATE   0x64

/*=======================================================
    FUNCTION
=========================================================*/
PUBLIC void Sys_restart(void) {

    IO_outB(PS2_STATE, SYS_C_RESET);

}

PUBLIC void Sys_panic(const char* str) {

    Console_setColor(CONSOLE_ERROR);
    Console_printf("%s%s", "[PANIC] ", str);
    Sys_disableInterrupts(); /* We don't want CPU to wake up */
    Sys_haltCPU();
}
