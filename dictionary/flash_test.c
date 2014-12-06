/*
 ************************************************************************
 ECE 362 - Mini-Project C Source File - Fall 2014
 ***********************************************************************
 */

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

#include "../course_includes/termio.h"


/* All functions after main should be initialized here */


/* Variable declarations */


/* Special ASCII characters */
#ifndef CR
	#define CR 0x0D		// ASCII return
#endif
#ifndef LF
	#define LF 0x0A		// ASCII new line
#endif

/*	 	   		
 ***********************************************************************
 Initializations
 ***********************************************************************
 */

void initializations(void)
{

	/* Set the PLL speed (bus clock = 24 MHz) */
	CLKSEL = CLKSEL & 0x80; //; disengage PLL from system
	PLLCTL = PLLCTL | 0x40; //; turn on PLL
	SYNR = 0x02; //; set PLL multiplier
	REFDV = 0; //; set PLL divider
	while(!(CRGFLG & 0x08))
	{
	}
	CLKSEL = CLKSEL | 0x80; //; engage PLL

	/* Disable watchdog timer (COPCTL register) */
	COPCTL = 0x40; //COP off; RTI and COP stopped in BDM-mode

	/* Initialize asynchronous serial port (SCI) for 9600 baud, interrupts off initially */
	SCIBDH = 0x00; //set baud rate to 9600
	SCIBDL = 0x9C; //24,000,000 / 16 / 156 = 9600 (approx)  
	SCICR1 = 0x00; //$9C = 156
	SCICR2 = 0x0C; //initialize SCI for program-driven operation
	DDRB = 0x10; //set PB4 for output mode
	PORTB = 0x10; //assert DTR pin on COM port

	/* Initialize peripherals */

	/* Initialize interrupts */


}

/*	 		  			 		  		
 ***********************************************************************
Main
 ***********************************************************************
 */
void main(void)
{
	DisableInterrupts
	initializations();
	EnableInterrupts;

	for(;;)
	{
		
	} /* loop forever */

} /* do not leave main */


/*
 ************************************************************************
 RTI interrupt service routine: RTI_ISR
 ************************************************************************
 */
interrupt 7 void RTI_ISR(void)
{
	// clear RTI interrupt flag
	CRGFLG = CRGFLG | 0x80;


}

/*
 ***********************************************************************
  TIM interrupt service routine	  		
 ***********************************************************************
 */
interrupt 15 void TIM_ISR(void)
{
	// clear TIM CH 7 interrupt flag 
	TFLG1 = TFLG1 | 0x80;


}

/*
 ***********************************************************************
  SCI interrupt service routine		 		  		
 ***********************************************************************
 */
interrupt 20 void SCI_ISR(void)
{



}