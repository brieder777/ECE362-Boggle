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

/* LCD COMMUNICATION BIT MASKS (note - different than previous labs) */
#define RS 0x10		// RS pin mask (PTT[4])
#define RW 0x20		// R/W pin mask (PTT[5])
#define LCDCLK 0x40	// LCD EN/CLK pin mask (PTT[6])

/* LCD INSTRUCTION CHARACTERS */
#define LCDON 0x0F	// LCD initialization command
#define LCDCLR 0x01	// LCD clear display command
#define TWOLINE 0x38	// LCD 2-line enable command
#define CURMOV 0xFE	// LCD cursor move instruction
    
// Line definitions (Line 3 and 4 = 1 and 2 on 2-line)
#define LINE1 0x00	// LCD line 1 cursor position
#define LINE2 0x40	// LCD line 2 cursor position
#define LINE3 0x80      // LCD line 3 cursor position
#define LINE4 0xC0      // LCD line 4 cursor position
    
// All character codes for the LCD are ASCII codes
    
/**
* Sets registers to allow for LCD functionality.
* 
* Instead of returning anything, this function initializes the LCD.
*
*/    
void lcd_2line_init(void);
    

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

