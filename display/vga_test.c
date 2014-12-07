/* 
 * File:   vga_test.c
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on November 26, 2014, 5:24 PM
 */

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

/* All functions after main should be initialized here */

/* Variable declarations */
char video_buffer[80*60 / 8];

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

	/* Set the PLL clock to 50 MHz (bus clock = 25 MHz) */
	CLKSEL = CLKSEL & 0x80; //; disengage PLL from system
	PLLCTL = PLLCTL | 0x40; //; turn on PLL
	
	// PLLCLK = 2 * OSCCLK * (SYNR + 1)/(REFDV + 1)
	// 50 MHz = 2 *  8 MHz * 25 / 8
	SYNR = 24;	// PLL multiplier
	REFDV = 7;	// PLL divider
			
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
	
	// Initialize TIM Ch 7 for periodic interrupts at 31.25 KHz
	TSCR1_TEN = 1;	// Enable timer subsystem
	TIOS_IOS7 = 1;	// Set Ch 7 to output compare mode
	TSCR2_PR = 0;	// No prescale
	TSCR2_TCRE = 1;	// Reset after OC7
	TC7 = 800;		// Scale clock to 25 MHz / 800 = 31.25 KHz
	TIE_C7I = 1;	// Enable interrupts

	/* Initialize interrupts */

	/* Initialize GPIO Pins */
	
	// Set HSYNC and VSYNC high.
	PTM_PTM0 = 1;
	PTM_PTM1 = 1;
	
	// Clear pixel bit.
	PTT = 0;
	
	// Set PM0, PM1, and PT7 as outputs.
	DDRM = 0x03;
	DDRT = 0x80;

}

extern void set_video_pointer(char* addr);

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
	
	set_video_pointer(video_buffer);

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
	// clear RTI interrupt flagt 
	CRGFLG = CRGFLG | 0x80;


}

extern void video_draw(void);

/*
 ***********************************************************************
  TIM interrupt service routine	  		
 ***********************************************************************
 */
interrupt 15 void TIM_ISR(void)
{
	video_draw();

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