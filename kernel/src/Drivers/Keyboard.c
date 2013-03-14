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
#include <Drivers/PS2Controller.h>
#include <Debug.h>
#include <X86/IDT.h>
#include <X86/PIC8259.h>

/*=======================================================
    DEFINE
=========================================================*/

#define KB_INT  33 /* PS/2 keyboard interrupt number(IRQ 1) */

/* Special keys */
#define KB_K_LCTRL      0x1D
#define KB_K_LALT       0x38
#define KB_K_LSHIFT     0x2A
#define KB_K_LSHIFT_OFF 0xAA
#define KB_K_RSHIFT     0x36
#define KB_K_F1         0x3B
#define KB_K_F2         0x3C
#define KB_K_F3         0x3D
#define KB_K_F4         0x3E
#define KB_K_F5         0x3F
#define KB_K_F6         0x40
#define KB_K_F7         0x41
#define KB_K_F8         0x42
#define KB_K_F9         0x43
#define KB_K_F10        0x44
#define KB_K_F11        0x57
#define KB_K_F12        0x58
#define KB_K_CAPS       0x3A
#define KB_K_NUM        0x45
#define KB_K_SCROLL     0x46

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
    STRUCT
=========================================================*/
typedef struct KeyState KeyState;
typedef struct LedStatusByte LedStatusByte;

struct KeyState {

    bool  scrollLock; /* Is scroll lock on? */
    bool  numberLock; /* Is number lock on? */
    bool  capsLock;   /* Is caps lock on? */
    bool  shift;      /* Is shift pressed? */

};

struct LedStatusByte {

    bool  scrollLock  : 1; /* Is scroll lock on? */
    bool  numberLock  : 1; /* Is number lock on? */
    bool  capsLock    : 1; /* Is caps lock on? */
    u8int             : 5;
};

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE KeyState keyState;

/* Scan code set 1 - shift or caps */
PRIVATE const u8int upperMap[256] = {

    0,  27, '!', '"', 0,  '$', '%', '^',  '&', '*', '(', ')', '_', '+', '\b', '\t', 'Q', 'W',  'E', 'R',
    'T', 'Y', 'U', 'I',  'O',  'P', '{', '}', '\n',   0, 'A', 'S', 'D', 'F',  'G',  'H', 'J', 'K',  'L', ':',
    '@', '~',  0, '~',  'Z',  'X', 'C', 'V',  'B', 'N', 'M', '<', '>', '?',   0,   '*',   0, ' ',    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0, '-',     0,   0,   0,  '+',   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0

};

/* Scan code set 1 - normal */
PRIVATE const u8int lowerMap[256] = {

    0,  27, '1', '2',  '3',  '4', '5', '6',  '7', '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w',  'e', 'r',
    't', 'y', 'u', 'i',  'o',  'p', '[', ']', '\n',   0, 'a', 's', 'd', 'f',  'g',  'h', 'j', 'k',  'l', ';',
    '\'', '`',  0, '#',  'z',  'x', 'c', 'v',  'b', 'n', 'm', ',', '.', '/',   0,   '*',   0, ' ',    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0, '-',     0,   0,   0,  '+',   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0,   0,   0,    0,   0,
    0,   0,   0,   0,   0,     0,   0,   0,    0,   0,   0,   0,   0,   0,   0,     0

};

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

    switch(scanCode) {

        case KB_K_LSHIFT:
            keyState.shift = TRUE;
            break;

        case KB_K_CAPS:
            keyState.capsLock = keyState.capsLock ? FALSE : TRUE;
            Keyboard_setLeds(keyState.numberLock, keyState.capsLock, keyState.scrollLock);
            break;

        case KB_K_NUM:
            keyState.numberLock = keyState.numberLock ? FALSE : TRUE;
            Keyboard_setLeds(keyState.numberLock, keyState.capsLock, keyState.scrollLock);
            break;

        case KB_K_SCROLL:
            keyState.scrollLock = keyState.scrollLock ? FALSE : TRUE;
            Keyboard_setLeds(keyState.numberLock, keyState.capsLock, keyState.scrollLock);
            break;

        case KB_K_LSHIFT_OFF:
            keyState.shift = FALSE;
            break;

        default:
            /* Check the top bit of scan code to see if a key has been released.
             * Process only if this is not a 'release' event */
            if(!(scanCode & 0x80)) {

                u8int b;

                if(keyState.shift || (keyState.capsLock && !keyState.shift))
                    b = upperMap[scanCode];
                else
                    b = lowerMap[scanCode];

                if(b)
                    Console_printChar(b);

            }

            break;

    }

}

PUBLIC void Keyboard_setLeds (bool numLock, bool capsLock, bool scrollLock) {

    LedStatusByte byte = {scrollLock, numLock, capsLock};

    Keyboard_sendCommand(KB_C_SET_LED);
    Keyboard_sendCommand(FORCE_CAST(byte, u8int));

    keyState.numberLock = numLock;
    keyState.capsLock = capsLock;
    keyState.scrollLock = scrollLock;
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

    /* set scan code set 1 */
    Keyboard_sendCommand(KB_C_SCAN_SET);
    Keyboard_sendCommand(1);

    /* Register keyboard IRQ handler */
    IDT_registerHandler(&Keyboard_callback, KB_INT);

    /* Unmask IRQ1 */
    PIC8259_setMask(1, CLEAR_MASK);

    /* Set leds to default state */
    Keyboard_setLeds(FALSE, FALSE, FALSE);
}
