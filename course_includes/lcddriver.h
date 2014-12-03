/* 
 * File:   lcddriver.h
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on October 31, 2014, 11:32 AM
 * 
 * Requires SPI to be initialized to 6 Mbps elsewhere!
 */

#ifndef LCDDRIVER_H
#define	LCDDRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif


/* LCD COMMUNICATION BIT MASKS (note - different than previous labs) */
#define RS 0x10		// RS pin mask (PTT[4])
#define RS_PIN PTT_PTT4
#define RW 0x20		// R/W pin mask (PTT[5])
#define RW_PIN PTT_PTT5
#define LCDCLK 0x40	// LCD EN/CLK pin mask (PTT[6])
#define LCDCLK_PIN PTT_PTT6

/* LCD INSTRUCTION CHARACTERS */
#define LCDON 0x0F	// LCD initialization command
#define LCDCLR 0x01	// LCD clear display command
#define TWOLINE 0x38	// LCD 2-line enable command
#define CURMOV 0xFE	// LCD cursor move instruction
#define LINE1 0x80	// LCD line 1 cursor position
#define LINE2 0xC0	// LCD line 2 cursor position
    
#ifndef CR
    /* ASCII character definitions */
    #define CR 0x0D	// ASCII return character
#endif


/**
 * Initialize the LCD.
 */
void lcd_init(void);

/*
 ***********************************************************************
  shiftout: Transmits the character x to external shift 
			register using the SPI.  It should shift MSB first.  
             
			MISO = PM[4]
			SCK  = PM[5]
 ***********************************************************************
 */
void shiftout(char x);

/*
 ***********************************************************************
  lcdwait: Delay for approx 2 ms
 ***********************************************************************
 */
void lcdwait(void);

/*
 *********************************************************************** 
  send_byte: writes character x to the LCD
 ***********************************************************************
 */
void send_byte(char x);

/*
 ***********************************************************************
  send_i: Sends instruction byte x to LCD  
 ***********************************************************************
 */
void send_i(char x);

/*
 ***********************************************************************
  chgline: Move LCD cursor to position x
  NOTE: Cursor positions are encoded in the LINE1/LINE2 variables
 ***********************************************************************
 */
void chgline(char x);

/*
 ***********************************************************************
  print_c: Print (single) character x on LCD            
 ***********************************************************************
 */
void print_c(char x);

/*
 ***********************************************************************
  pmsglcd: print character string str[] on LCD
 ***********************************************************************
 */
void pmsglcd(char[]);

/*
 * Print a number on the LCD.
 */
void pnumlcd(unsigned int number, unsigned char digits);


#ifdef	__cplusplus
}
#endif

#endif	/* LCDDRIVER_H */

