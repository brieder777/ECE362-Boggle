
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>


/* All functions after main should be initialized here */
void lcd_shiftout(char x);
void lcdwait(void);
void lcd_send_byte(char x,char lcd);
void lcd_send_i(char x,char lcd);
void lcd_chgline(char loc,char lcd);
void lcd_print_c(char x, char lcd);
void lcd_message(char x[], char lcd);
void delay(unsigned int x);

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

char LINE1 = 0x80;     //;LCD line 1 cursor position
char LINE2 = 0xC0;     //;LCD line 2 cursor position
char LINE3 = 0x94;
char LINE4 = 0xD4;
	 	   		
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


  DDRT_DDRT4 = 1;//pt4 as output(/cs)	 
  DDRT_DDRT5 = 1;//pt5 as output(clk1)	 
  DDRT_DDRT6 = 1;//pt6 as output(clk2)	 
  DDRT_DDRT7 = 1;//pt7 as output(clk3)	 
  RS = 1;//initially /RS is negated
  DDRAD= 0xFF;//configure ptad as output  

/* initialize the lcds*/  
  LCDCLK1 = 1;             //pull LCDCLK high (idle)
  LCDCLK2 = 1; 
  LCDCLK3 = 1; 
  lcd_send_i(LCDON,1);            //turn on LCD (LCDON instruction) ($0F)
  lcd_send_i(LCDON,2);            //turn on LCD (LCDON instruction) ($0F)
  lcd_send_i(TWOLINE,1);          //enable two-line mode (TWOLINE instruction) ($38)
  lcd_send_i(TWOLINE,2);          //enable two-line mode (TWOLINE instruction) ($38)
  lcd_send_i(LCDCLR,1);           //clear LCD (LCDCLR instruction) ($01) 
  lcd_send_i(LCDCLR,2);           //clear LCD (LCDCLR instruction) ($01)
  delay(8000);                //wait for 2ms so that the LCD can wake up       
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
	//lcd_message(small,2);
	//lcd_print_c('h',1);
	
  for(;;) {
  
/* < start of your main loop > */   

  } /* loop forever */
   
}   /* do not leave main */

/*
* lcd_shiftout(char x): put x on port ad (as data for lcd)
*/
void lcd_shiftout(char x)
{
  PTAD = x;
}

void clock(char lcd) 
{ //pulse LCD clock line low->high->low for appropriate lcd
  if(lcd == 1) {
    LCDCLK1 = 0;
    LCDCLK1 = 1;
    LCDCLK1 = 0;
  } else if(lcd == 2) {
    LCDCLK2 = 0;
    LCDCLK2 = 1;
    LCDCLK2 = 0;
  } else if(lcd == 3){
    LCDCLK2 = 0;
    LCDCLK2 = 1;
    LCDCLK2 = 0;
  }
}

void lcdwait()
{  //wait for lcd
  delay(200);
}
void delay(unsigned int x) {
  int i;
  for(i=0;i<x;i++);
}

/*
*********************************************************************** 
  send_byte: writes character x to an LCD chosen by lcd 
***********************************************************************
*/

void lcd_send_byte(char x, char lcd)
{
  lcd_shiftout(x);   // shift out character
  clock(lcd); // pulse LCD clock line low->high->low
  lcdwait();     // wait 2 ms for LCD to process data
}

/*
***********************************************************************
  send_i: Sends instruction byte x to an LCD chosen by lcd  
***********************************************************************
*/

void lcd_send_i(char x, char lcd)
{
  RS = 0;   // set the register select line low (instruction data)
  lcd_send_byte(x,lcd);   // send byte
}

/*
***********************************************************************
  chgline: Move LCD cursor to position x for lcd
  NOTE: Cursor positions are encoded in the LINE1/LINE2/LINE3/LINE4 variables
***********************************************************************
*/

void lcd_chgline(char pos, char lcd)
{
  lcd_send_i(0xFE,lcd); //send CURMOV instruction ($FE)
  lcd_send_byte(pos, lcd);
}

/*
***********************************************************************
  print_c: Print (single) character x on an LCD chosen by lcd           
***********************************************************************
*/
 
void lcd_print_c(char x, char lcd)
{
  RS = 1;   // set the register select line high (byte data)
  lcd_send_byte(x,lcd);   // send byte
}
/*
***********************************************************************
  lcd_message: Print string of characters x on LCD chosen by lcd            
***********************************************************************
*/
void lcd_message(char x[], char lcd)
{
  int i;
  for(i=0;x[i]!='\0';i++) { //iterate until end of string
    lcd_print_c(x[i],lcd);         //print each character
  }  
}
