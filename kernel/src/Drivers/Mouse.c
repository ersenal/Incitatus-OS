/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Mouse.c
|--------------------------------------------------------------------------
|
| DESCRIPTION: Interrupt based PS/2 mouse driver.
|
| AUTHOR:      Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Drivers/Mouse.h>
#include <Common.h>
#include <Drivers/PS2Controller.h>
#include <Debug.h>
#include <X86/IDT.h>
#include <X86/PIC8259.h>
#pragma GCC diagnostic ignored "-Wstrict-aliasing" /* ignore FORCE_CAST compiler warning */

/*=======================================================
    DEFINE
=========================================================*/

#define MS_INT 44 /* ps/2 mouse interrupt number(IRQ 12) */

/** Command Bytes - sent from PS/2 controller to mouse */
#define MS_C_SELECT_PORT_2 0xD4 /* next byte will be sent to port 2(mouse) */
#define MS_C_RESET         0xFF /* resets mouse */
#define MS_C_ENABLE_AUX    0xA8 /* enables auxiliary mouse device */
#define MS_C_GET_COMPAQ    0x20 /* retrieve compaq status byte */
#define MS_C_SET_COMPAQ    0x60 /* sets the compaq status byte */
#define MS_C_DEFAULT       0xF6 /* default mouse settings */
#define MS_C_ENABLE        0xF4 /* enables packet streaming */
#define MS_C_DISABLE       0xF5 /* disables packet streaming */

/** Response Bytes - sent from mouse to PS/2 controller */
#define MS_R_ACK           0xFA /* command acknowledged */

/** Debug Messages */
#define MS_M_DEFAULT_1  "Mouse set to default"
#define MS_M_DEFAULT_0  "Mouse could not be set to default"
#define MS_M_ENABLE_1   "Mouse packet streaming enabled"
#define MS_M_ENABLE_0   "Mouse packet streaming could not be enabled"

/*=======================================================
    STRUCTS
=========================================================*/
typedef struct MByte0 MByte0;
typedef struct MByte1 MByte1;
typedef struct MByte2 MByte2;
typedef struct MousePacket MousePacket;

struct MByte0 {

    u8int leftBtn   : 1; /* is left button being pressed? */
    u8int rightBtn  : 1; /* is right button  being pressed? */
    u8int middleBtn : 1; /* is middle button being pressed? */
    u8int always1   : 1;
    u8int xSign     : 1; /* indicates MByte1 sign */
    u8int ySign     : 1; /* indicates MByte2 sign */
    u8int xOverflow : 1;
    u8int yOverflow : 1;

} __attribute__((packed));

struct MByte1 {
    u8int xMovement : 8;
} __attribute__((packed));

struct MByte2 {
    u8int yMovement : 8;
} __attribute__((packed));

struct MousePacket {

    MByte0 byte0;
    MByte1 byte1;
    MByte2 byte2;

};

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE MousePacket packet;
PRIVATE u8int cycle;

/*=======================================================
    FUNCTION
=========================================================*/

/** No commands are SENT to the mouse after enabling interrupts(IRQ)
 *  should receive an ack after sending a command */
PRIVATE bool Mouse_sendCommand(u8int command) {

    PS2Controller_send(PS2_STATE, MS_C_SELECT_PORT_2);
    PS2Controller_send(PS2_DATA, command);

    if(PS2Controller_receive(PS2_DATA) != MS_R_ACK) return 0;
    else return 1;

}

PRIVATE void Mouse_callback(void) {

    u8int b = PS2Controller_receive(PS2_DATA);

    switch(cycle) {

        case 0:
        packet.byte0 = FORCE_CAST(b, MByte0);
        cycle++;
        break;

        case 1:
        packet.byte1 = FORCE_CAST(b, MByte1);
        cycle++;
        break;

        case 2:
        packet.byte2 = FORCE_CAST(b, MByte2);
        cycle = 0;

        /* discard if overflows are set */
        if(packet.byte0.xOverflow || packet.byte0.yOverflow) break;

        /* mouse packet ready to use */
        // Console_printf("%d%c%d", packet.byte1.xMovement, ',', packet.byte2.yMovement);
        break;

    }

}

PUBLIC void Mouse_init(void) {

    Debug_logInfo("%s", "Initialising Mouse");

    /* Enable auxiliary mouse device */
    PS2Controller_send(PS2_STATE,MS_C_ENABLE_AUX);
    PS2Controller_receive(PS2_DATA); /* flush */

    /* tell mouse to use default settings */
    if(Mouse_sendCommand(MS_C_DEFAULT)) {

        Debug_logInfo("%s", MS_M_DEFAULT_1);

    } else {

        Debug_logError("%s", MS_M_DEFAULT_0);

    }

    /* Enable packet streaming */
    if(Mouse_sendCommand(MS_C_ENABLE)) {

        Debug_logInfo("%s", MS_M_ENABLE_1);

    } else {

        Debug_logError("%s", MS_M_ENABLE_0);

    }

    IDT_registerHandler(&Mouse_callback, MS_INT);

    /* Unmask IRQ12 */
    PIC8259_setMask(12, CLEAR_MASK);

    /* Also unmask IRQ2 - cascade IRQ(enables access to slave IRQs ranging 8 - 15) */
    PIC8259_setMask(2, CLEAR_MASK);

}