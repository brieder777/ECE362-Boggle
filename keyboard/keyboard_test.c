/*
 ************************************************************************
 ECE 362 - Mini-Project C Source File - Fall 2014
 ***********************************************************************
 */

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

#include "../Course Includes/lcddriver.h"

/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);


/* Variable declarations */



/* Special ASCII characters */
//#define CR 0x0D		// ASCII return�
//#define LF 0x0A		// ASCII new line�

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
	//  SCIBDH =  0x00; //set baud rate to 9600
	//  SCIBDL =  0x9C; //24,000,000 / 16 / 156 = 9600 (approx)  
	SCICR1 = 0x00; //$9C = 156
	SCICR2 = 0x0C; //initialize SCI for program-driven operation
	DDRB = 0x10; //set PB4 for output mode
	PORTB = 0x10; //assert DTR pin on COM port

	SCICR1_PE = 1; // Parity enable
	SCICR1_PT = 1; // Odd Parity

//	SCICR2_RE = 1;
	
	SCIBD = 117; // 24MHz / 16 / 117 = 12853 Hz

	/* Initialize peripherals */

	/* Initialize interrupts */

	/*
	  Initialize SPI for baud rate of 6 Mbs, MSB first
	  (note that R/S, R/W', and LCD clk are on different PTT pins)
	 */
	//	SPICR1_SPE = 1; // Enable SPI
	//	SPICR1_MSTR = 1; // Master Mode
	//	SPICR1_CPHA = 0; // Send data on odd edges
	//	SPICR1 = 0x50;
//	SPICR2 = 0; // Clear SPICR2 for good practice (single-direction mode)

//	SPICR1_SPE = 1;
//	SPICR1_MSTR = 0; // Slave Mode
//	SPICR1_CPHA = 0; // Receive data on odd edges
//	SPICR1_CPOL = 1;
	
	/*
	  Initialize SPI for baud rate of 6 Mbs, MSB first
	  (note that R/S, R/W', and LCD clk are on different PTT pins)
	 */
//	SPICR1_SPE = 1; // Enable SPI
//	SPICR1_MSTR = 1; // Master Mode
//	SPICR1_CPHA = 0; // Send data on odd edges
	SPICR1 = 0x50;
	SPICR2 = 0; // Clear SPICR2 for good practice (single-direction mode)


	DDRT = 0xFF;
	DDRM &= 0xF0;


}

/*	 		  			 		  		
 ***********************************************************************
Main
 ***********************************************************************
 */
char lastc = 0;
void main(void)
{
	char last = 1;
	char byte = 0;
	unsigned int x = 0;

	DisableInterrupts
	initializations();
	EnableInterrupts;
	
	lcd_init();

	// GPIO test
	//	 PTT = 0b01010101;
	
	for(;;)
	{	
		// SCI
		char c = inchar();
		if(c != 0 && c != 4 && c != lastc)
		{
			pnumlcd(c, 3);
			print_c(' ');
			lastc = c;
		}
		if(c == 0)
		{
			lastc = 0;
			c=inchar();//read second byte of break code
		}
		

			 
		//chgline(LINE2);
		//pnumlcd(inchar(),3);
//		if(SCISR1_NF) // If error
//			pnumlcd(1, 3);
//		else if(SCISR1_PF)
//			pnumlcd(2, 1);
//		else if (SCISR1_FE)
//			pnumlcd(3, 1);
//		else if (SCISR1_OR)
//			pnumlcd(4, 1);
		
		//chgline(LINE1);


		// Manual
//		while(x--);
//		
//		if(PTM_PTM0 && !last)
//		{
//			byte = byte << 1;
//			byte |= PTM_PTM1;
//		}
//		last = PTM_PTM0;
//
//		PTT = byte;


		// SPI
//		while(SPISR_SPIF);
//		PTT = SPIDR;

	} /* loop forever */

} /* do not leave main */




//interrupt 19 void SPI_ISR(void)
//{
//	
//}

/*
 *********************************************************************** 
 RTI interrupt service routine: RTI_ISR
 ************************************************************************
 */

interrupt 7 void RTI_ISR(void)
{
	// clear RTI interrupt flagt 
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

/*
 ***********************************************************************
 Character I/O Library Routines for 9S12C32 
 ***********************************************************************
 Name:         inchar
 Description:  inputs ASCII character from SCI serial port and returns it
 Example:      char ch1 = inchar();
 ***********************************************************************
 */

char inchar(void)
{
	/* receives character from the terminal channel */
	while(!(SCISR1 & 0x20)); /* wait for input */
	return SCIDRL;
}

/*
 ***********************************************************************
 Name:         outchar    (use only for DEBUGGING purposes)
 Description:  outputs ASCII character x to SCI serial port
 Example:      outchar('x');
 ***********************************************************************
 */

void outchar(char x)
{
	/* sends a character to the terminal channel */
	while(!(SCISR1 & 0x80)); /* wait for output buffer empty */
	SCIDRL = x;
}