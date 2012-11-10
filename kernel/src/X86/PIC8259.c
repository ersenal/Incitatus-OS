/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| 8259PIC.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Provides an interface for programmable interrupt controller
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
|
|               TYPE comments source:
|                   http://stanislavs.org/helppc/8259.html
\------------------------------------------------------------------------*/


#include <X86/PIC8259.h>
#include <IO.h>
#include <Debug.h>

/*=======================================================
    DEFINE
=========================================================*/

/* PIC ports */
#define MASTER_COMMAND 0x20
#define MASTER_DATA    0x21
#define SLAVE_COMMAND  0xA0
#define SLAVE_DATA     0xA1

/* End of interrupt*/
#define EOI 0x20

/*=======================================================
    TYPE
=========================================================*/

/* Initialization Command Word

Initialization Command Word 1 at Port 20h and A0h

    |7|6|5|4|3|2|1|0|  ICW1
    | | | | | | | `---- 1=ICW4 is needed, 0=no ICW4 needed
    | | | | | | `----- 1=single 8259, 0=cascading 8259's
    | | | | | `------ 1=4 byte interrupt vectors, 0=8 byte int vectors
    | | | | `------- 1=level triggered mode, 0=edge triggered mode
    | | | `-------- must be 1 for ICW1 (port must also be 20h or A0h)
    `------------- must be zero for PC systems

Initialization Command Word 2 at Port 21h and A1h

    |7|6|5|4|3|2|1|0|  ICW2
    | | | | | `-------- 000= on 80x86 systems
    `----------------- A7-A3 of 80x86 interrupt vector

Initialization Command Word 3 at Port 21h and A1h

    |7|6|5|4|3|2|1|0|  ICW3 for Master Device
    | | | | | | | `---- 1=interrupt request 0 has slave, 0=no slave
    | | | | | | `----- 1=interrupt request 1 has slave, 0=no slave
    | | | | | `------ 1=interrupt request 2 has slave, 0=no slave
    | | | | `------- 1=interrupt request 3 has slave, 0=no slave
    | | | `-------- 1=interrupt request 4 has slave, 0=no slave
    | | `--------- 1=interrupt request 5 has slave, 0=no slave
    | `---------- 1=interrupt request 6 has slave, 0=no slave
    `----------- 1=interrupt request 7 has slave, 0=no slave

    |7|6|5|4|3|2|1|0|  ICW3 for Slave Device
    | | | | | `-------- master interrupt request slave is attached to
    `----------------- must be zero

Initialization Command Word 4 at Port 21h and A1h

    |7|6|5|4|3|2|1|0|  ICW4
    | | | | | | | `---- 1 for 80x86 mode, 0 = MCS 80/85 mode
    | | | | | | `----- 1 = auto EOI, 0=normal EOI
    | | | | `-------- slave/master buffered mode (see below)
    | | | `--------- 1 = special fully nested mode (SFNM), 0=sequential
    `-------------- unused (set to zero)

    Bits Buffering Mode

     00 not buffered
     01 not buffered
     10 buffered mode slave (PC mode)
     11 buffered mode master (PC mode)
*/
typedef unsigned char ICW;

/* Operation Control Word

Operation Control Word 1 / Interrupt Mask Reg.  (Ports 21h & A1h)

    |7|6|5|4|3|2|1|0|  OCW1 - IMR Interrupt Mask Register
    | | | | | | | `---- 0 = service IRQ0, 1 = mask off
    | | | | | | `----- 0 = service IRQ1, 1 = mask off
    | | | | | `------ 0 = service IRQ2, 1 = mask off
    | | | | `------- 0 = service IRQ3, 1 = mask off
    | | | `-------- 0 = service IRQ4, 1 = mask off
    | | `--------- 0 = service IRQ5, 1 = mask off
    | `---------- 0 = service IRQ6, 1 = mask off
    `----------- 0 = service IRQ7, 1 = mask off


Operation Control Word 2 / Interrupt Command Reg. (Ports 20h & A0h)

    |7|6|5|4|3|2|1|0|  OCW2 - ICR Interrupt Command Register
    | | | | | `-------- interrupt request level to act upon
    | | | | `--------- must be 0 for OCW2
    | | | `---------- must be 0 for OCW2
    `--------------- EOI type (see table)

    Bits EOI - End Of Interrupt code (PC specific)

    001  non-specific EOI command
    010  NOP
    011  specific EOI command
    100  rotate in automatic EOI mode
    101  rotate on non-specific EOI command
    110  set priority command  (uses bits 2-0)
    111  rotate on specific EOI command


Operation Control Word 3   (Ports 20h & A0h)

    |7|6|5|4|3|2|1|0|  OCW3
    | | | | | | | `--- 1=read IRR on next read, 0=read ISR on next read
    | | | | | | `---- 1=act on value of bit 0, 0=no action if bit 0 set
    | | | | | `----- 1=poll command issued, 0=no poll command issued
    | | | | `------ must be 1 for OCW3
    | | | `------- must be 0 for OCW3
    | | `-------- 1=set special mask, 0=reset special mask
    | `--------- 1=act on value of bit 5, 0=no action if bit 5 set
    `---------- not used (zero)
*/
typedef unsigned char OCW;

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module picModule;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void PIC8259_init(void) {

    /* IRQ 0-7 normally use INT 8-15, which conflicts with the exception INTs.
    *  Remap IRQ so that IRQ 0-15 correspond to INT 32-47.
    */

    /* Start initialisation sequence */
    ICW icw1 = 0b00010001;
    IO_outB(MASTER_COMMAND, icw1);
    IO_outB(SLAVE_COMMAND,  icw1);

    /* Master PIC and Slave PIC vector offset (32 and 47) */
    ICW icw2 = 0b00100000;
    IO_outB(MASTER_DATA, icw2);
    icw2 = 0b00101000;
    IO_outB(SLAVE_DATA,  icw2);

    /* Tell Master there is a Slave at IRQ2*/
    ICW icw3 = 0b00000100;
    IO_outB(MASTER_DATA, icw3);
    /* Tell Slave its cascade identity */
    icw3 = 0b00000010;
    IO_outB(SLAVE_DATA, icw3);

    /* 8086 mode */
    ICW icw4 = 0b00000001;
    IO_outB(MASTER_DATA, icw4);
    IO_outB(SLAVE_DATA, icw4);

    /* Initially mask all IRQs */
    OCW ocw1 = 0b00000000;
    IO_outB(MASTER_DATA, ocw1);
    IO_outB(SLAVE_DATA, ocw1);

}

PUBLIC void PIC8259_setMask(u8int irqNo, bool state) {

    u16int port;
    OCW ocw1;

    if (irqNo < 8) { /* Master PIC */

        port = MASTER_DATA;

    } else { /* SLAVE PIC */

        port = SLAVE_DATA;
        irqNo -= 8;

    }

    if(state == 0) /* Clear mask */
        ocw1 = IO_inB(port) & ~(1 << irqNo);
    else /* Set mask */
        ocw1 = IO_inB(port) | (1 << irqNo);

    IO_outB(port, ocw1);
}

PUBLIC void PIC8259_sendEOI(u8int interruptNo) {

    if (interruptNo >= 40) /* If it came from the slave (IRQ8 - 15) */
        IO_outB(SLAVE_COMMAND, EOI);

    IO_outB(MASTER_COMMAND, EOI);

}

PUBLIC Module* PIC8259_getModule(void) {

    picModule.moduleName = "8259A PIC";
    picModule.init = &PIC8259_init;
    picModule.numberOfDependencies = 0;
    picModule.moduleID = MODULE_PIC8259;

    return &picModule;
}