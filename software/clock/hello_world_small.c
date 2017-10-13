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
#include "alt_types.h"

#define TimerStatus ((volatile short*) (TIMER_0_BASE))
#define TimerControl ((volatile short*) (TIMER_0_BASE+4))
#define TimerTimeoutL ((volatile short*) (TIMER_0_BASE+8))
#define TimerTimeoutH ((volatile short*) (TIMER_0_BASE+12))
#define TimerSnapshotL ((volatile short*) (TIMER_0_BASE+16))
#define TimerSnapshotH ((volatile short*) (TIMER_0_BASE+20))


unsigned long numclks,seconds,minutes;
unsigned long numchigh,numclow;


volatile int edge_capture;

int* led = LED_BASE;
int* modo = MODO_BASE;

unsigned long *min_d = MINUTOS_D_BASE;
unsigned long *min_u = MINUTOS_U_BASE;

unsigned long *seg_d = HORAS_D_BASE;
unsigned long *seg_u = HORAS_U_BASE;

unsigned long *hora_d = SEGUNDOS_U_BASE;
unsigned long *hora_u = SEGUNDOS_D_BASE;

unsigned long minutos_reales = 0;
unsigned long horas_reales = 0;

unsigned long minutos_alarm = 0;
unsigned long horas_alarm = 0;


int* botones = PUSH_BUTTONS_BASE;

static void handle_timer_interrupts(void* context, alt_u32 id)
{
	*led = 1;
	*(TimerControl)=6;
}


void display_time()
{





		seconds = 59-seconds;
		display(min_d, minutos_reales % 10);
		display(min_u, minutos_reales / 10);
		display(seg_u, seconds % 10);
		display(seg_d, seconds / 10);
		display(hora_u, horas_reales % 10);
		display(hora_d, horas_reales / 10);



	//printf("Seconds_d %lu :  seconds_u %lu   \n", minutes,seconds);
	//display(u_seg, seconds % 10);
	//display(d_seg, seconds / 10);


}

void display(unsigned long* ptr , unsigned long number)
{
	if(number == 0)
		* ptr= 64;
	if(number == 1)
		* ptr= 121;
	if(number == 2)
		* ptr= 36;
	if(number == 3)
		* ptr= 48;
	if(number == 4)
		* ptr= 25;
	if(number == 5)
		* ptr= 18;
	if(number == 6)
		* ptr= 2;
	if(number == 7)
		* ptr= 120;
	if(number == 8)
		* ptr= 0;
	if(number == 9)
		* ptr=16;

}


void modoAlarma()
{
	void* edge_capture_ptr = (void*) &edge_capture;
				//register interrupt
	alt_irq_register( TIMER_0_IRQ,	edge_capture_ptr,handle_timer_interrupts );


		   // Configure the timeout period to maximum
	*(TimerTimeoutL)=0x5e00;
	*(TimerTimeoutH)=0xb2d0;
		   // Configure timer to start counting and stop in cero
	*(TimerControl)=6;
		    //*(TimerControl)=2;

	*led = 0;
	while (*modo == 0)
	{
	   *(TimerSnapshotL)=0; //write to timer to get snapshot
	    numclow = *(TimerSnapshotL); //get low part
	   numchigh = *(TimerSnapshotH); //get high part
	   numclks = numclow | (numchigh << 16); //assemble full number
	   seconds = numclks/50000000 ;
		  // printf("Botones %d",*botones);
	   if(*botones == 7)
	   {

		   configurar_alarma();
	   }
	   if(seconds == 45)
	   {
	   	   *led = 0;
	   }


		  minutes = seconds/60;
		  seconds = seconds-minutes*60;
		 if(  seconds != 25 ) //corriendo, (RUN && !STOP)
		   {
			   if(*(TimerStatus) == 3)
			   {

				   minutos_reales = minutos_reales+1 ;

				   if((minutos_reales == minutos_alarm) && (horas_reales == horas_alarm))
				   {

					   *(TimerControl)=5;
				   }
				   *(TimerStatus) =  2;

			   }
			   if(minutos_reales == 60)
			   {
				   if(minutos_reales == 1)
					   *(TimerControl)=5;
				   minutos_reales-=60;
				   horas_reales++;
			   }

		    	display_time();

		   }

		 }
	modoReloj();


}



void configurar_reloj()
{
	while(1)
	{
		if(*botones == 11)
		{

			break;
		}
		if(*botones == 13)
		{
			horas_reales++;
			if(horas_reales == 24)
			{
				horas_reales = 0;
			}
		}
		if(*botones == 14)
		{
			minutos_reales++;
			if(minutos_reales == 60)
			{
				minutos_reales = 0;

			}
		}
		seconds = 59;

		display_time();
		usleep(200000);

	}
}



void configurar_alarma()
{
	while(1)
	{
		if(*botones == 11)
		{

			break;
		}
		if(*botones == 13)
		{
			horas_alarm++;
			if(horas_alarm == 24)
			{
				horas_alarm = 0;
			}
		}
		if(*botones == 14)
		{
			minutos_alarm++;
			if(minutos_alarm == 60)
			{
				minutos_alarm = 0;

			}
		}

		display(min_d, minutos_alarm % 10);
		display(min_u, minutos_alarm / 10);
		display(seg_u, 0);
		display(seg_d, 0);
		display(hora_u, horas_alarm % 10);
		display(hora_d, horas_alarm / 10);
		usleep(200000);

	}
	//minutos_alarm =0;
	//horas_alarm = 0;
}
void modoReloj()
{

	alt_putstr("Hello from Nios II!\n");
	   // Configure the timeout period to maximum
	*(TimerTimeoutL)=0x5e00;
	*(TimerTimeoutH)=0xb2d0;
	   // Configure timer to start counting and stop in cero
	*(TimerControl)=6;
	    //*(TimerControl)=2;

	*led = 0;
	while (*modo == 1)
	{
	   *(TimerSnapshotL)=0; //write to timer to get snapshot
	    numclow = *(TimerSnapshotL); //get low part
	   numchigh = *(TimerSnapshotH); //get high part
	   numclks = numclow | (numchigh << 16); //assemble full number
	   seconds = numclks/50000000 ;
	  // printf("Botones %d",*botones);
	   if(*botones == 7)
	   {
		   printf("Botones %d",*botones);
		   configurar_reloj();
	   }

	   minutes = seconds/60;
	   seconds = seconds-minutes*60;
	   if(  seconds != 25 ) //corriendo, (RUN && !STOP)
	   {
		   if(*(TimerStatus) == 3)
		   {
			   *(TimerStatus) =  2;
			   minutos_reales = minutos_reales+1 ;
		   }
		   if(minutos_reales == 60)
		   {
			   if(minutos_reales == 1)
				   *(TimerControl)=5;
			   minutos_reales-=60;
			   horas_reales++;
		   }
	    	display_time();
	   }

	 }
	modoAlarma();

}



int main()
{
	minutos_reales = 0;
	horas_reales = 0;
	if(*modo == 0)
	{
		modoAlarma();
	}
	else
		modoReloj();

  return 0;
}
