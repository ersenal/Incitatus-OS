/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| PS2Controller.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  PS/2 Controller driver.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef PS2_H
#define PS2_H

#include <Common.h>
#include <Module.h>

/*=======================================================
    DEFINE
=========================================================*/

/* PS/2 IO ports */
#define PS2_DATA     0x60 /* used for reading/writing data from PS/2 device or PS/2 controller */
#define PS2_PORTB    0x61 /* used for controlling various parts of the chipset */
#define PS2_STATE    0x64 /* used for retrieving/changing the state of PS/2 controller */

/*=======================================================
    FUNCTION
=========================================================*/

/*-------------------------------------------------------------------------
| Send PS/2 command
|--------------------------------------------------------------------------
| DESCRIPTION:  Sends a command byte to specified PS/2 port.
|
| PARAM:        "port"      ps/2 port to send command to
|               "command"   command byte
\------------------------------------------------------------------------*/
void PS2Controller_send(u8int port, u8int command);

/*-------------------------------------------------------------------------
| Receive PS/2 data
|--------------------------------------------------------------------------
| DESCRIPTION:  Retrieves a data byte from "port".
|
| PARAM:        "port"  port to receive data from
|
| RETURN:       u8int   retrieved data byte
\------------------------------------------------------------------------*/
u8int PS2Controller_receive(u8int port);

/*-------------------------------------------------------------------------
| Get PS/2 controller module
|--------------------------------------------------------------------------
| DESCRIPTION:    Returns the PS/2 controller module.
|
\------------------------------------------------------------------------*/
Module* PS2Controller_getModule(void);
#endif
