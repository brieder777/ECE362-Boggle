
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>
#include "lcd.h"


/* All functions after main should be initialized here */


/* Variable declarations */
   	   			 		  			 		       

/* Special ASCII characters */
#define CR 0x0D		// ASCII return 
#define LF 0x0A		// ASCII new line 

/* LCD COMMUNICATION BIT MASKS (note - different than previous labs) */
#define RS PTT_PTT4		// RS pin mask (PTT[4])
#define LCDCLK1 PTT_PTT5	// LCD1 EN/CLK pin mask (PTT[5])
#define LCDCLK2 PTT_PTT6	// LCD2 EN/CLK pin mask (PTT[6])
#define LCDCLK3 PTT_PTT7	// LCD3 EN/CLK pin mask (PTT[7])
#define LCDON 0x0F	// LCD initialization command
#define LCDCLR 0x01	// LCD clear display command
#define TWOLINE 0x38	// LCD 2-line enable command
#define CURMOV 0xFE	// LCD cursor move instruction

#define LINE1 0x80     //;LCD line 1 cursor position
#define LINE2 0xC0     //;LCD line 2 cursor position
#define LINE3 0x94
#define LINE4 0xD4
	 	   		
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


   

/* initialize the lcds*/ 

  lcdinit(); 
       
}

	 		  			 		  		
/*	 		  			 		  		
***********************************************************************
Main
***********************************************************************
*/
void main(void) {

  char big[4]="big";
  char small[6]="small";
	DisableInterrupts;
	initializations();
	EnableInterrupts;
	
	lcd_chgline(LINE3,1);
	//lcd_chgline(0xC0,2);
	lcd_message(big,1);
  lcd_chgline(LINE2,2);
	lcd_message(small,2);
  lcd_chgline(LINE2,3);
  lcd_message("test",3);
	//lcd_print_c('h',1);
	
  for(;;) {
  
/* < start of your main loop > */   

  } /* loop forever */
   
}   /* do not leave main */

