/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Keyboard.c
|--------------------------------------------------------------------------
|
| DESCRIPTION: Interrupt based PS/2 keyboard driver.
|
| AUTHOR:      Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Drivers/Keyboard.h>
#include <Common.h>
#include <Drivers/PS2Controller.h>
#include <Debug.h>
#include <X86/IDT.h>
#include <X86/PIC8259.h>

/*=======================================================
    DEFINE
=========================================================*/

#define KB_INT  33 /* PS/2 keyboard interrupt number(IRQ 1) */

/** Command Bytes - sent from PS/2 controller to keyboard */
#define KB_C_SET_LED        0xED /* accompanied by a data byte (SL, NL, CL) */
#define KB_C_ECHO           0xEE /* keyboard echoes back 0xEE */
#define KB_C_SCAN_SET       0xF0 /* Data byte ('0' - get current scan set, '1..3' - set scan code set '1..3') */
#define KB_C_IDENTIFY       0xF2 /* Keyboard sends back identifier bytes */
#define KB_C_DELAY          0xF3
#define KB_C_SCAN_ENABLE    0xF4 /* enables scanning */
#define KB_C_SCAN_DISABLE   0xF5 /* disables scanning */
#define KB_C_DEFAULT        0xF6 /* set default parameters */
#define KB_C_RESEND         0xFE /* tell keyboard to send the previously sent byte */
#define KB_C_RESET          0xFF /* tell keyboard to reset and start self-test */

/** Response Bytes - sent from keyboard to PS/2 controller */
#define KB_R_ERROR          0x00 /* key detection error or internal buffer overrun */
#define KB_R_ERROR2         0xFF /* key detection error or internal buffer overrun */
#define KB_R_TEST_PASSED    0xAA /* self test passed, sent after KB_C_RESET */
#define KB_R_TEST_FAILED    0xFC /* self test failed, sent after KB_C_RESET */
#define KB_R_TEST_FAILED2   0xFD /* self test failed, sent after KB_C_RESET */
#define KB_R_ECHO           0xEE /* response to KB_C_ECHO */
#define KB_R_ACK            0xFA /* command acknowledged */
#define KB_R_RESEND         0xFE /* repeat the last command */

/** Debug Messages */
#define KB_M_SCAN_DISABLE_1 "Keyboard scan disabled"
#define KB_M_SCAN_DISABLE_0 "Keyboard scan could not be disabled"
#define KB_M_TEST_PASSED_1  "Keyboard self-test passed"
#define KB_M_TEST_PASSED_0  "Keyboard self-test failed"
#define KB_M_SCAN_ENABLE_1  "Keyboard scan enabled"
#define KB_M_SCAN_ENABLE_0  "Keyboard scan could not be enabled"

/*=======================================================
    FUNCTION
=========================================================*/

/** No commands are SENT to the keyboard after enabling interrupts(IRQ)
 *  should receive an ack after sending a command */
PRIVATE bool Keyboard_sendCommand(u8int command) {

    PS2Controller_send(PS2_DATA, command);

    if(PS2Controller_receive(PS2_DATA) != KB_R_ACK) return 0;
    else return 1;

}

/*-------------------------------------------------------------------------
| Keyboard high level interrupt handler
|--------------------------------------------------------------------------
| DESCRIPTION:     Whenever a PS/2 keyboard interrupt gets raised, this
|                  callback function is called by common IRQ handler.
|                  (refer to IDT.c : IDT_handlerIRQ)
|
| NOTES:           Keyboard IRQ: 1(Interrupt 33)
\------------------------------------------------------------------------*/
PRIVATE void Keyboard_callback(void) {

    u8int scanCode = PS2Controller_receive(PS2_DATA); /* get the pressed scan code */
    Console_printChar(scanCode);

}

PUBLIC void Keyboard_init(void) {

    /* disable scanning, should receive ACK */
    if(Keyboard_sendCommand(KB_C_SCAN_DISABLE)) {
        Debug_logInfo("%s", KB_M_SCAN_DISABLE_1);
    } else {
        Debug_logError("%s", KB_M_SCAN_DISABLE_0);
    }

    /* reset device and self-test */
    /* should receive 0xFA then 0xAA from device(ACK and self-test passed) */
    if(Keyboard_sendCommand(KB_C_RESET))  {

        if(PS2Controller_receive(PS2_DATA) == KB_R_TEST_PASSED) {
            Debug_logInfo("%s", KB_M_TEST_PASSED_1);
        }

    } else {
        Debug_logError("%s", KB_M_TEST_PASSED_0);
    }

    /* enable scanning, should receive ACK */
    if(Keyboard_sendCommand(KB_C_SCAN_ENABLE)) {
        Debug_logInfo("%s", KB_M_SCAN_ENABLE_1);
    } else {
        Debug_logError("%s", KB_M_SCAN_ENABLE_0);
    }

    IDT_registerHandler(&Keyboard_callback, KB_INT);

    /* Unmask IRQ1 */
    PIC8259_setMask(1, CLEAR_MASK);

}
