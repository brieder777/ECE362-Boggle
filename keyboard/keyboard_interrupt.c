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

//@TODO: keyboard_init

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>
#include "keyboard.h"

union {
    int word;
    struct {
      unsigned char high;
      unsigned char low;
    } bytes;
  } keyboard_frame;

/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);

/* Variable declarations */ 
static unsigned char keyboard_char_buff;      

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
  DDRT = 1;

/* Initialize peripherals */
            
/* Initialize interrupts */
	 INTCR_IRQE = 0;
   INTCR_IRQEN = 1;
	 PTM_PTM1 = 1;
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
    if(keyboard_char_buff)
    {
      outchar(translate_keyboard_character(keyboard_char_buff));

      keyboard_char_buff = 0;
    }
    
  
   } /* loop forever */
   
}   /* do not leave main */

/*
***********************************************************************                       
  IRQ triggered interrupt service routine       
***********************************************************************
*/

unsigned char translate_keyboard_character(unsigned char buff_char)
{
  // outbin(buff_char);
  switch(buff_char) {
    case KEYBOARD_A:
      return 'A';
    case KEYBOARD_B:
      return 'B';
    case KEYBOARD_C:
      return 'C';
    case KEYBOARD_D:
      return 'D';
    case KEYBOARD_E:
      return 'E';
    case KEYBOARD_F:
      return 'F';
    case KEYBOARD_G:
      return 'G';
    case KEYBOARD_H:
      return 'H';
    case KEYBOARD_I:
      return 'I';
    case KEYBOARD_J:
      return 'J';
    case KEYBOARD_K:
      return 'K';
    case KEYBOARD_L:
      return 'L';
    case KEYBOARD_M:
      return 'M';
    case KEYBOARD_N:
      return 'N';
    case KEYBOARD_O:
      return 'O';
    case KEYBOARD_P:
      return 'P';
    case KEYBOARD_Q:
      return 'Q';
    case KEYBOARD_R:
      return 'R';
    case KEYBOARD_S:
      return 'S';
    case KEYBOARD_T:
      return 'T';
    case KEYBOARD_U:
      return 'U';
    case KEYBOARD_V:
      return 'V';
    case KEYBOARD_W:
      return 'W';
    case KEYBOARD_X:
      return 'X';
    case KEYBOARD_Y:
      return 'Y';
    case KEYBOARD_Z:
      return 'Z';
    case KEYBOARD_ENTER:
      return '\n';
    case KEYBOARD_SPACE:
      return ' ';
    case KEYBOARD_BACKSPACE:
      return '\b';
    default:
      return '\0';
  }
}

static unsigned char count = 0;
static unsigned char breakflag = 0;

interrupt 6 void IRQ_ISR(void)
{
  // Send clear signal to flip-flop -- high, low, high
  PTM_PTM1 = 1;
  PTM_PTM1 = 0;
  PTM_PTM1 = 1;

  // Shift in only the bytes between start and parity
  if(count >= 1 && count <= 8)
  {
    keyboard_frame.bytes.high = PTM_PTM0;
    keyboard_frame.word = keyboard_frame.word >> 1;
  }

  // After 11 bits have been read, send the character out
  if(count == 10)
  {
    if(breakflag==1)
      breakflag = 0;
    else if(keyboard_frame.bytes.low == 0xF0)
      breakflag = 1;
    else
      keyboard_char_buff = keyboard_frame.bytes.low;
  } 
  
  // Increment number of bits read with modulus
  count = (count + 1) % 11;
}

/*
***********************************************************************                       
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

void outbin_noout(char x)
{
  unsigned char itr;
  for(itr = 0; itr < 8; ++itr) {
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
