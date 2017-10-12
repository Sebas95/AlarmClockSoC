/* 
 * "Small Hello World" example. 
 * 
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example 
 * designs. It requires a STDOUT  device in your system's hardware. 
 *
 * The purpose of this example is to demonstrate the smallest possible Hello 
 * World application, using the Nios II HAL library.  The memory footprint
 * of this hosted application is ~332 bytes by default using the standard 
 * reference design.  For a more fully featured Hello World application
 * example, see the example titled "Hello World".
 *
 * The memory footprint of this example has been reduced by making the
 * following changes to the normal "Hello World" example.
 * Check in the Nios II Software Developers Manual for a more complete 
 * description.
 * 
 * In the SW Application project (small_hello_world):
 *
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 * In System Library project (small_hello_world_syslib):
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 *    - Define the preprocessor option ALT_NO_INSTRUCTION_EMULATION 
 *      This removes software exception handling, which means that you cannot 
 *      run code compiled for Nios II cpu with a hardware multiplier on a core 
 *      without a the multiply unit. Check the Nios II Software Developers 
 *      Manual for more details.
 *
 *  - In the System Library page:
 *    - Set Periodic system timer and Timestamp timer to none
 *      This prevents the automatic inclusion of the timer driver.
 *
 *    - Set Max file descriptors to 4
 *      This reduces the size of the file handle pool.
 *
 *    - Check Main function does not exit
 *    - Uncheck Clean exit (flush buffers)
 *      This removes the unneeded call to exit when main returns, since it
 *      won't.
 *
 *    - Check Don't use C++
 *      This builds without the C++ support code.
 *
 *    - Check Small C library
 *      This uses a reduced functionality C library, which lacks  
 *      support for buffering, file IO, floating point and getch(), etc. 
 *      Check the Nios II Software Developers Manual for a complete list.
 *
 *    - Check Reduced device drivers
 *      This uses reduced functionality drivers if they're available. For the
 *      standard design this means you get polled UART and JTAG UART drivers,
 *      no support for the LCD driver and you lose the ability to program 
 *      CFI compliant flash devices.
 *
 *    - Check Access device drivers directly
 *      This bypasses the device file system to access device drivers directly.
 *      This eliminates the space required for the device file system services.
 *      It also provides a HAL version of libc services that access the drivers
 *      directly, further reducing space. Only a limited number of libc
 *      functions are available in this configuration.
 *
 *    - Use ALT versions of stdio routines:
 *
 *           Function                  Description
 *        ===============  =====================================
 *        alt_printf       Only supports %s, %x, and %c ( < 1 Kbyte)
 *        alt_putstr       Smaller overhead than puts with direct drivers
 *                         Note this function doesn't add a newline.
 *        alt_putchar      Smaller overhead than putchar with direct drivers
 *        alt_getchar      Smaller overhead than getchar with direct drivers
 *
 */

#include "sys/alt_stdio.h"
#include "system.h"
#import <stdio.h>
#include <stdlib.h>

#define TimerStatus ((volatile short*) (TIMER_0_BASE))
#define TimerControl ((volatile short*) (TIMER_0_BASE+4))
#define TimerTimeoutL ((volatile short*) (TIMER_0_BASE+8))
#define TimerTimeoutH ((volatile short*) (TIMER_0_BASE+12))
#define TimerSnapshotL ((volatile short*) (TIMER_0_BASE+16))
#define TimerSnapshotH ((volatile short*) (TIMER_0_BASE+20))


unsigned long numclks,seconds,minutes;
unsigned long numchigh,numclow;

int main()
{ 
  alt_putstr("Hello from Nios II!\n");



     // Configure the timeout period to maximum
     *(TimerTimeoutL)=0xffff;
     *(TimerTimeoutH)=0xffff;
     // Configure timer to start counting and stop in cero
     *(TimerControl)=4;
     //*(TimerControl)=2;
     int* led = LED_BASE;
     while (1)
     {
        *(TimerSnapshotL)=0; //write to timer to get snapshot
        numclow = *(TimerSnapshotL); //get low part
        numchigh = *(TimerSnapshotH); //get high part
        numclks = numclow | (numchigh << 16); //assemble full number
        seconds = numclks/50000000 ;

        minutes = seconds/60;
        seconds = seconds-minutes*60;
        if(*(TimerStatus)==2 &  seconds != 25 ) //corriendo, (RUN && !STOP)
        {
        	printf("Minutes %lu :  seconds %lu   \n", minutes,seconds);
        	*led = 0;
        	//*(TimerControl)=8;
        }
        if(*(TimerStatus)==1) //reached to 0 and stopped (!RUN && STOP)
        {

        	printf("Estado :%d \n", *(TimerStatus));
            printf("%lu  : \n", numclks);
            *led = 1;
            //*(TimerControl)=8; //stop timer
         }


     }

  return 0;
}
