/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| PS2Controller.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  PS/2 Controller driver.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Drivers/PS2Controller.h>
#include <IO.h>
#include <Debug.h>
#include <Drivers/Keyboard.h>
#include <Drivers/Mouse.h>
#pragma GCC diagnostic ignored "-Wstrict-aliasing" /* ignore FORCE_CAST compiler warning */

/*=======================================================
    DEFINE
=========================================================*/

#define POLL_COUNTER 100000;

/** PS/2 device types */
#define PS2_MOUSE_STANDARD  0x00 /* mouse WITHOUT a scroll wheel */
#define PS2_MOUSE_SCROLL    0x03 /* mouse WITH a scroll wheel */
#define PS2_MOUSE_MULTI     0x04 /* 5-button mouse */
#define PS2_KB              0xAB /* should receive this as first byte if the device is keyboard */
#define PS2_KB_NORMAL       0x83 /* should receive this as second byte if the keyboard has translation disabled */

/** PS/2 Controller commands
 *  Use PS2_STATE if 1 byte
 *  Use PS2_STATE then PS2_DATA if 2 bytes*/
#define PS2_C_DISABLE_1     0xAD /* disable first PS/2 port(keyboard) */
#define PS2_C_DISABLE_2     0xA7 /* disable second PS/2 port(mouse) */
#define PS2_C_ENABLE_1      0xAE /* enable first PS/2 port(keyboard) */
#define PS2_C_ENABLE_2      0xA8 /* enable second PS/2 port(mouse) */
#define PS2_C_TEST          0xAA /* test the controller itself */
#define PS2_C_TEST_1        0xAB /* test first PS/2 port(keyboard) */
#define PS2_C_TEST_2        0xA9 /* test second PS/2 port(mouse) */
#define PS2_C_DIAG          0xAC /* dumps all bytes of internal RAM */
#define PS2_C_READ_CONF     0x20 /* returns byte 0(configuration byte) from internal ram */
#define PS2_C_WRITE_CONF    0x60 /* write next byte to byte 0(configuration byte) */
#define PS2_C_READ_OUT      0xD0 /* read controller output port */
#define PS2_C_WRITE_OUT     0xD1 /* write next byte to controller output port */

/** PS/2 Controller response */
#define PS2_R_TEST_PASSED       0x55 /* self-test passed */
#define PS2_R_TEST_FAILED       0xFC /* self-test failed */
#define PS2_R_TEST_PORT_PASSED  0x00 /* port-test passed */

/** Debug Messages */
#define PS2_M_DISABLE       "Disabling ports"
#define PS2_M_DISABLE_INT   "Disabling interrupts"
#define PS2_M_TEST_1        "Controller self-test passed"
#define PS2_M_TEST_0        "Controller self-test failed"
#define PS2_M_TEST_1_1      "Port-1 self-test passed"
#define PS2_M_TEST_1_0      "Port-1 self-test failed"
#define PS2_M_TEST_2_1      "Port-2 self-test passed"
#define PS2_M_TEST_2_0      "Port-2 self-test failed"
#define PS2_M_ENABLE        "Enabling ports"
#define PS2_M_ENABLE_INT    "Enabling port interrupts"

/*=======================================================
    STRUCT
=========================================================*/
typedef struct PS2ConfigByte PS2ConfigByte;

/** Configuration Byte(byte 0 in internal controller ram) */
struct PS2ConfigByte {

    u8int firstPortInterruptFlag    : 1; /* 1 = enabled, 0 = disabled */
    u8int secondPortInterruptFlag   : 1; /* 1 = enabled, 0 = disabled */
    u8int systemFlag                : 1; /* 1 = system passed POST */
    u8int                           : 1; /* Always zero */
    u8int firstPortClock            : 1; /* 1 = disabled, 0 = enabled */
    u8int secondPortClock           : 1; /* 1 = disabled, 0 = enabled */
    u8int firstPortTranslation      : 1; /* 1 = enabled,  0 = disabled */
    u8int                           : 1; /* Always zero */

} __attribute__((packed));

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE PS2ConfigByte configByte;
PRIVATE Module ps2Module;

/*=======================================================
    FUNCTION
=========================================================*/

/* Poll the PS/2 controller  until the controller output buffer is full */
PRIVATE void PS2Controller_pollOutput(void) {

    int count = POLL_COUNTER;

    /* check if controller output buffer is full and return */
    while(!(IO_inB(PS2_STATE) & 0x1) && count) count--;
}

/* Poll the PS/2 controller  until the controller input buffer is empty */
PRIVATE void PS2Controller_pollInput(void) {

    int count = POLL_COUNTER;

    /* check if controller input buffer is empty and return */
    while((IO_inB(PS2_STATE) & 0x2) && count) count--;
}

PRIVATE void PS2Controller_readConfigByte(void) {

    PS2Controller_send(PS2_STATE, PS2_C_READ_CONF);
    u8int b = PS2Controller_receive(PS2_DATA);
    configByte = FORCE_CAST(b, PS2ConfigByte);

}

PRIVATE void PS2Controller_writeConfigByte(void) {

    PS2Controller_send(PS2_STATE, PS2_C_WRITE_CONF);
    PS2Controller_send(PS2_DATA, FORCE_CAST(configByte, u8int));

}

PRIVATE void PS2Controller_init(void) {

    /* disable both PS/2 ports */
    Debug_logInfo("%s", PS2_M_DISABLE);
    PS2Controller_send(PS2_STATE, PS2_C_DISABLE_1);
    PS2Controller_send(PS2_STATE, PS2_C_DISABLE_2);

    /* flush controller's output buffer */
    PS2Controller_receive(PS2_DATA);

    /* we need to set the config byte. read the current one and change the required bits */
    /* disable all IRQs and traslation(clear bits 0, 1, 6) */
    Debug_logInfo("%s", PS2_M_DISABLE_INT);
    PS2Controller_readConfigByte();
    configByte.firstPortInterruptFlag = 0;
    configByte.secondPortInterruptFlag = 0;
    configByte.firstPortTranslation = 0;
    PS2Controller_writeConfigByte();

    /* do controller self-test */
    PS2Controller_send(PS2_STATE, PS2_C_TEST);
    if(PS2Controller_receive(PS2_DATA) == PS2_R_TEST_PASSED) {
        Debug_logInfo("%s", PS2_M_TEST_1);
    } else {
        Debug_logError("%s", PS2_M_TEST_0);
    }

    /* do port tests */
    PS2Controller_send(PS2_STATE, PS2_C_TEST_1);
    if(PS2Controller_receive(PS2_DATA) == PS2_R_TEST_PORT_PASSED) {
        Debug_logInfo("%s", PS2_M_TEST_1_1);
    } else {
        Debug_logError("%s", PS2_M_TEST_1_0);
    }

    PS2Controller_send(PS2_STATE, PS2_C_TEST_2);
    if(PS2Controller_receive(PS2_DATA) == PS2_R_TEST_PORT_PASSED) {
        Debug_logInfo("%s", PS2_M_TEST_2_1);
    } else {
        Debug_logError("%s", PS2_M_TEST_2_0);
    }


    /* enable ports */
    Debug_logInfo("%s", PS2_M_ENABLE);
    PS2Controller_send(PS2_STATE, PS2_C_ENABLE_1);
    PS2Controller_send(PS2_STATE, PS2_C_ENABLE_2);

    /* Initialise PS/2 devices */
    Keyboard_init();
    Mouse_init();

    /* we can now enable port interrupts, IRQ */
    Debug_logInfo("%s", PS2_M_ENABLE_INT);
    PS2Controller_readConfigByte();
    configByte.firstPortInterruptFlag = 1;
    configByte.secondPortInterruptFlag = 1;
    PS2Controller_writeConfigByte();
}

PUBLIC void PS2Controller_send(u8int port, u8int command) {

    PS2Controller_pollInput();
    IO_outB(port, command);

}

PUBLIC u8int PS2Controller_receive(u8int port) {

    PS2Controller_pollOutput();
    return IO_inB(port);

}

PUBLIC Module* PS2Controller_getModule(void) {

    if(!ps2Module.isLoaded) {

        ps2Module.moduleName = "PS/2 Controller";
        ps2Module.moduleID = MODULE_PS2;
        ps2Module.init = &PS2Controller_init;
        ps2Module.numberOfDependencies = 1;
        ps2Module.dependencies[0] = MODULE_IDT;

    }

    return &ps2Module;

}
