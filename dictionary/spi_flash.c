
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>
#include "spi_flash.h"

void spi_flash_init(void)
{
	/* Initialize peripherals */
	SPICR1 = 0x50; //interrupts disabled, power on, master mode, active high clk,msb first
	SPICR2 = 0; //disable bi-directional mode
	SPIBR = 0; // No scale - set baud rate to 12 Mbs (sppr=0, spr=0)

	/* Initialize interrupts */
	DDRT_DDRT3 = 1; //pt3 as output(/cs)	 
	PTT_PTT3 = 1; //initially /cs is negated   
}

char spi_flash_read_addr(long addr)
{
	union Address a;
	char ret;
	
	a.addr = addr;
	CS_PIN = 0; //assert /cs to signal start of instruction
	spi_flash_shiftout(spi_flash_read);
	spi_flash_shiftout(a.bytes[1]); //1st addr byte 
	spi_flash_shiftout(a.bytes[2]); //2nd addr byte
	spi_flash_shiftout(a.bytes[3]); //3rd addr byte
	
	// We need at least 5 cycles of delay here for reasons unknown. :(
	asm{
		nop
		nop
		nop
		nop
		nop
	}
	spi_flash_shiftin(); //clear spif
	
	spi_flash_shiftout(0xFF);
	ret = spi_flash_shiftin();
	
	CS_PIN = 1; //negate /cs to signal end of instruction

	return ret;
}

char spi_flash_shiftin() 
{
  while(SPISR_SPIF != 1);//wait for data to shift in
  return SPIDR;
}

/*
 * Read a word terminated with a newline, string starting at addr.
 */
void spi_flash_read_word(long addr, char* buffer)
{
	char read;
	
	read = spi_flash_read_addr(addr);
	while(read != '\n')
	{
		*buffer = read;
		buffer++;
		
		addr++;
		read = spi_flash_read_addr(addr);
	}
	
	*buffer = '\0';
}

/*
 * Look for the next newline character, then read the word after it to buffer.
 */
void spi_flash_read_current_word(long addr, char* buffer)
{
	char read;
	
	read = spi_flash_read_addr(addr);
	while(read != '\n')
	{
		addr++;
		read = spi_flash_read_addr(addr);
	}
	
	spi_flash_read_word(addr + 1, buffer);
}

void spi_flash_shiftout(char x)
{
  while(SPISR_SPTEF != 1);//read the SPTEF bit, continue if bit is 1
  SPIDR = x;                 //write data to SPI data register
//  delay(5);
}