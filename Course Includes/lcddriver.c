/* 
 * File:   lcddriver.c
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on October 31, 2014, 11:35 AM
 */

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

#include "lcddriver.h"

void lcd_init()
{
	LCDCLK_PIN = 1;
	RW_PIN = 0;
	send_i(LCDON);
	send_i(TWOLINE);
	send_i(LCDCLR);
	lcdwait();
}

void shiftout(char x)
{
	// read the SPTEF bit, continue if bit is 1
	while(SPISR_SPTEF == 0);
	
	// write data to SPI data register
	SPIDR = x;
	
	// wait for 30 cycles for SPI data to shift out 
	lcdwait();
}

void lcdwait()
{
	// The DNBE instruction takes 3 cycles.
	// At 24 MHz, 16,000 instructions = 48,000 cycles ~= 2ms
	asm {
		ldx #16000
		loop: dbne X,loop
	}
}

void send_byte(char x)
{
	// shift out character
	shiftout(x);
	
	// pulse LCD clock line low->high->low
	LCDCLK_PIN = 1;
	LCDCLK_PIN = 0;
	LCDCLK_PIN = 1;
	
	// wait 2 ms for LCD to process data
	lcdwait();
}

void send_i(char x)
{
	// set the register select line low (instruction data)
	RS_PIN = 0;
	
	// send byte
	send_byte(x);
}

void chgline(char x)
{
	send_i(CURMOV);
	send_i(x);
}

void print_c(char x)
{
	RS_PIN = 1; // Set the register select to print mode.
	
	send_byte(x);
}

void pmsglcd(char str[])
{
	RS_PIN = 1; // Set the register select to print mode.
	
	while(*str != '\0')
	{
		send_byte(*str);
		str++;
	}
}

void pnumlcd(unsigned int number, unsigned char digits)
{
	char string[6];
	
	string[digits] = '\0';
	
	while(digits > 0)
	{
		string[--digits] = (number % 10) + '0';
		number /= 10;
	}
	
	pmsglcd(string);
}
