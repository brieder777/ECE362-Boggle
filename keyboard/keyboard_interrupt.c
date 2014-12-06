/*
************************************************************************
 ECE 362 - Mini-Project C Source File - Fall 2014                    
***********************************************************************
	 	   			 		  			 		  		
 Team ID: < ? >

 Project Name: < ? >

 Team Members:

   - Team/Doc Leader: < ? >      Signature: ______________________
   
   - Software Leader: < ? >      Signature: ______________________

   - Interface Leader: < ? >     Signature: ______________________

   - Peripheral Leader: < ? >    Signature: ______________________


 Academic Honesty Statement:  In signing above, we hereby certify that we 
 are the individuals who created this HC(S)12 source file and that we have
 not copied the work of any other student (past or present) while completing 
 it. We understand that if we fail to honor this agreement, we will receive 
 a grade of ZERO and be subject to possible disciplinary action.

***********************************************************************

 The objective of this Mini-Project is to .... < ? >


***********************************************************************

 List of project-specific success criteria (functionality that will be
 demonstrated):

 1.

 2.

 3.

 4.

 5.

***********************************************************************

  Date code started: < ? >

  Update history (add an entry every time a significant change is made):

  Date: < ? >  Name: < ? >   Update: < ? >

  Date: < ? >  Name: < ? >   Update: < ? >

  Date: < ? >  Name: < ? >   Update: < ? >


***********************************************************************
*/

//@TODO: Print out characters based on reverse integers

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);
void outbin(char x);
void outbin_int(int x);

/* Variable declarations */
unsigned char keyboard_bit = 0;
int keyboard_character = 0;
unsigned char keyboard_buff_itr = 0;
unsigned char keyboard_char_buff[10];
   	   			 		  			 		       

/* Special ASCII characters */
#define CR 0x0D		// ASCII return 
#define LF 0x0A		// ASCII new line 

/* LCD COMMUNICATION BIT MASKS (note - different than previous labs) */
#define RS 0x10		// RS pin mask (PTT[4])
#define RW 0x20		// R/W pin mask (PTT[5])
#define LCDCLK 0x40	// LCD EN/CLK pin mask (PTT[6])

/* LCD INSTRUCTION CHARACTERS */
#define LCDON 0x0F	// LCD initialization command
#define LCDCLR 0x01	// LCD clear display command
#define TWOLINE 0x38	// LCD 2-line enable command
#define CURMOV 0xFE	// LCD cursor move instruction
#define LINE1 = 0x80	// LCD line 1 cursor position
#define LINE2 = 0xC0	// LCD line 2 cursor position

	 	   		
/*	 	   		
***********************************************************************
 Initializations
***********************************************************************
*/

void  initializations(void) {

/* Set the PLL speed (bus clock = 24 MHz) */
  CLKSEL = CLKSEL & 0x80; //; disengage PLL from system
  PLLCTL = PLLCTL | 0x40; //; turn on PLL
  SYNR = 0x02;            //; set PLL multiplier
  REFDV = 0;              //; set PLL divider
  while (!(CRGFLG & 0x08)){  }
  CLKSEL = CLKSEL | 0x80; //; engage PLL

/* Disable watchdog timer (COPCTL register) */
  COPCTL = 0x40   ; //COP off; RTI and COP stopped in BDM-mode

/* Initialize asynchronous serial port (SCI) for 9600 baud, interrupts off initially */
  SCIBDH =  0x00; //set baud rate to 9600
  SCIBDL =  0x9C; //24,000,000 / 16 / 156 = 9600 (approx)  
  SCICR1 =  0x00; //$9C = 156
  SCICR2 =  0x0C; //initialize SCI for program-driven operation
  DDRB   =  0x10; //set PB4 for output mode
  PORTB  =  0x10; //assert DTR pin on COM port

  //Keyboard initializations
  // Initialize the Port M 0 and 1 as input and output respectively
  DDRM_DDRM0 = 0; // Data received from keyboard
  DDRM_DDRM1 = 1; // CLR signal sent to flip-flop

/* Initialize peripherals */
            
/* Initialize interrupts */
	 INTCR_IRQE = 0;
   INTCR_IRQEN = 1;
	      
}

	 		  			 		  		
/*	 		  			 		  		
***********************************************************************
Main
***********************************************************************
*/
void main(void) {
  	DisableInterrupts
	initializations(); 		  			 		  		
	EnableInterrupts;

 for(;;) {
  
/* < start of your main loop > */ 
  
    
  
   } /* loop forever */
   
}   /* do not leave main */

/*
***********************************************************************                       
  IRQ triggered interrupt service routine       
***********************************************************************
*/

void keyboard_char_to_buff(void)
{
  unsigned char itr;
  //keyboard_character = keyboard_character >> 1;
  keyboard_char_buff[keyboard_buff_itr] = 0x00;


  for(itr = 0; itr < 8; ++itr) {
    keyboard_char_buff[keyboard_buff_itr] = keyboard_char_buff[keyboard_buff_itr] << 1;
    keyboard_char_buff[keyboard_buff_itr] |= (keyboard_character & 0x01);
    keyboard_character = keyboard_character >> 1;
  }
  
  //keyboard_char_buff[keyboard_buff_itr] |= (keyboard_character & 0xFF);
}

interrupt 6 void IRQ_ISR(void)
{
  // Send clear signal to flip-flop -- high, low, high
  PTM_PTM1 = 1;
  PTM_PTM1 = 0;
  PTM_PTM1 = 1;

  // The character has to be reset after every 11th bit
  if(keyboard_bit == 11) {
    keyboard_bit = 0;
    //keyboard_char_to_buff();
    outbin_int(keyboard_character);
    keyboard_character = 0;
    //outbin(keyboard_char_buff[keyboard_buff_itr]);
    ++keyboard_buff_itr;
  }

  // Read data in from the keyboard
  keyboard_character = keyboard_character << 1;
  keyboard_character |= PTM_PTM0;
  ++keyboard_bit;
}

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

char inchar(void) {
  /* receives character from the terminal channel */
        while (!(SCISR1 & 0x20)); /* wait for input */
    return SCIDRL;
}

/*
***********************************************************************
 Name:         outchar    (use only for DEBUGGING purposes)
 Description:  outputs ASCII character x to SCI serial port
 Example:      outchar('x');
***********************************************************************
*/

void outbin_int(int x)
{
  unsigned char itr;
  for(itr = 0; itr < 16; ++itr) {
    outchar((x & 0x01) + '0');
    x = x >> 1;
  }
  outchar('\r');
  outchar('\n');
}

void outbin(char x)
{
  unsigned char itr;
  for(itr = 0; itr < 8; ++itr) {
    outchar((x & 0x01) + '0');
    x = x >> 1;
  }
  outchar('\r');
  outchar('\n');
}

void outchar(char x) {
  /* sends a character to the terminal channel */
    while (!(SCISR1 & 0x80));  /* wait for output buffer empty */
    SCIDRL = x;
}
