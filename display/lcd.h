/* 
 * File:   lcd.h
 * Author: Brian Rieder <brieder@purdue.edu>
 *
 * Created on December 8, 2014, 10:32 PM
 * 
 * The datasheet for the LCD for which this driver is written can be found
 * at: https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 * 
 * ============================================================================
 * 
 * This library was made to be compatible with the CodeWarrior compiler.
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

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
    
/**
 * Pulses LCD clock line low->high->low for appropriate LCD
 *
 * @param lcd       A character ('1' or '2') that designates the desired
 *					LCD to clock.
 */
void lcd_clock(char lcd);

/*
* Outputs character on port AD as data for LCD
*
* @param x          A character that should be output on the clock
*/
void lcd_shiftout(char x);

/*
* Delays to wait for the LCD 200 times
*/
void lcdwait(void);

/*
*********************************************************************** 
  send_byte: writes character x to an LCD chosen by lcd 
***********************************************************************
*/
void lcd_send_byte(char x,char lcd);

/*
***********************************************************************
  send_i: Sends instruction byte x to an LCD chosen by lcd  
***********************************************************************
*/

void lcd_send_i(char x,char lcd);

/*
***********************************************************************
  chgline: Move LCD cursor to position x for lcd
  NOTE: Cursor positions are encoded in the LINE1/LINE2/LINE3/LINE4 variables
***********************************************************************
*/
void lcd_chgline(char loc,char lcd);

/*
***********************************************************************
  print_c: Print (single) character x on an LCD chosen by lcd           
***********************************************************************
*/
void lcd_print_c(char x, char lcd);

/*
***********************************************************************
  lcd_message: Print string of characters x on LCD chosen by lcd            
***********************************************************************
*/
void lcd_message(char x[], char lcd);

/*
* Iterates through a for loop x times to create delay.
*
* @param x          Number of times to iterate through the loop
*/
void delay(unsigned int x);

/**
 * Initialize the LCD.
 */
void lcdinit(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

