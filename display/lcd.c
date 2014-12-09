/* 
 * File:   lcd.c
 * Author: Brian Rieder <brieder@purdue.edu>
 *
 * Created on December 8, 2014, 10:33 PM
 */

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>
#include "lcd.h"

void lcd_2line_init() 
{
    /* Initialize the LCD
     - pull LCDCLK high (idle)
     - pull R/W' low (write state)
     - turn on LCD (LCDON instruction)
     - enable two-line mode (TWOLINE instruction)
     - clear LCD (LCDCLR instruction)
     - wait for 2ms so that the LCD can wake up     
     */
    
}