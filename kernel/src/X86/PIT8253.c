/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| PIT8253.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Provides an interface for Intel 8253 Programmable Interval
|               Timer(PIT).
|
|               PIT consists of three timers(channels). Each has a different
|               purpose.
|
|               First Timer  - Used as the System timer
|               Second Timer - Used for RAM refreshing
|               Third Timer  - Connected to PC speaker(that annoying beeper)
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
|
|               TYPE comments source:
|                   http://stanislavs.org/helppc/8259.html
\------------------------------------------------------------------------*/


#include <X86/PIT8253.h>
#include <IO.h>
#include <X86/IDT.h>
#include <X86/PIC8259.h>
#include <Process/ProcessManager.h>
#include <Sys.h>

/*=======================================================
    DEFINE
=========================================================*/

/* 8253 Register I/O Ports */
#define PORT_CHANNEL_0  0x40 /* System timer - connected to PIC IRQ0, generates an interrupt every clock tick. */
#define PORT_CHANNEL_1  0x41 /* We do not utilise this channel */
#define PORT_CHANNEL_2  0x42 /* We do not utilise this channel */
#define PORT_CONTROL    0x43 /* Sets PIT operation modes - Write only register */

/* The system timer fires IRQ0 every 10 ms
 *
 *  DIVIDER = (INPUT_HZ / OUTPUT_HZ) = 11931.8
 *  (INPUT_HZ / DIVIDER) = 100Hz, each clock tick 10ms
 */
#define INPUT_HZ   1193180 /* Input frequency */
#define OUTPUT_HZ  100     /* Output frequency */

/*=======================================================
    TYPE
=========================================================*/

/*  Control Register

|7|6|5|4|3|2|1|0|  Mode Control Register
 | | | | | | | `---- 0=16 binary counter, 1=4 decade BCD counter
 | | | | `--------- counter mode bits
 | | `------------ read/write/latch format bits
 `--------------- counter select bits

Bits Counter Select Bits
 00  select counter 0
 01  select counter 1
 10  select counter 2

Bits Read/Write/Latch Format Bits
 00  latch present counter value
 01  read/write of MSB only
 10  read/write of LSB only
 11  read/write LSB, followed by write of MSB

Bits  Counter Mode Bits
 000  mode 0, interrupt on terminal count;  countdown, interrupt,
      then wait for a new mode or count; loading a new count in the
      middle of a count stops the countdown
 001  mode 1, programmable one-shot; countdown with optional
      restart; reloading the counter will not affect the countdown
      until after the following trigger
 010  mode 2, rate generator; generate one pulse after 'count' CLK
      cycles; output remains high until after the new countdown has
      begun; reloading the count mid-period does not take affect
      until after the period
 011  mode 3, square wave rate generator; generate one pulse after
      'count' CLK cycles; output remains high until 1/2 of the next
      countdown; it does this by decrementing by 2 until zero, at
      which time it lowers the output signal, reloads the counter
      and counts down again until interrupting at 0; reloading the
      count mid-period does not take affect until after the period
 100  mode 4, software triggered strobe; countdown with output high
      until counter zero;  at zero output goes low for one CLK
      period;  countdown is triggered by loading counter;  reloading
      counter takes effect on next CLK pulse
 101  mode 5, hardware triggered strobe; countdown after triggering
      with output high until counter zero; at zero output goes low
*/
typedef u8int ControlRegister;

/*=======================================================
    PRIVATE DATA
=========================================================*/
PRIVATE Module pitModule;
PRIVATE u32int tick;

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE void PIT8253_timerHandler(Regs* regs) {

    tick++;

    //if(tick % 100 == 0) /* context switch every 20ms */
    ProcessManager_switch(regs);

}

PRIVATE void PIT8253_init(void) {

    u16int divisor = INPUT_HZ / OUTPUT_HZ;

    /* Set channel 0 control mode */
    ControlRegister cr = 0b00110110;
    IO_outB(PORT_CONTROL, cr);

    /* Send the frequency divisor. */
    IO_outB(PORT_CHANNEL_0, divisor); /* Send lower byte */
    IO_outB(PORT_CHANNEL_0, divisor >> 8); /* Send higher byte */

    /* Register timer IRQ0 handler */
    IDT_registerHandler(&PIT8253_timerHandler, IRQ0);

    /* Unmask IRQ0 */
    PIC8259_setMask(0, 0);

}

PUBLIC u32int PIT8253_measureRuntime(void* functionAddr) {

    u32int beforeTick = tick;

    void (*function) (void) = functionAddr;
    function();

    return tick - beforeTick;

}

/* GCC without O0 tends to optimise busy waits, prevent that for this function */
#pragma GCC push_options
#pragma GCC optimize ("O0")
PUBLIC void PIT8253_sleep(u32int ms) {

    ms /= 10;
    u32int begin = tick;
    while(tick < (ms + begin))
      Sys_haltCPU();

}
#pragma GCC pop_options

PUBLIC Module* PIT8253_getModule(void) {

    pitModule.moduleName = "8253 PIT";
    pitModule.moduleID = MODULE_PIT8253;
    pitModule.init = &PIT8253_init;
    pitModule.numberOfDependencies = 1;
    pitModule.dependencies[0] = MODULE_IDT;

    return &pitModule;
}
