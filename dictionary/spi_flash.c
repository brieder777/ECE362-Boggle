
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>
#include "spi_flash.h"
#include <string.h>

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

char spi_flash_shiftin() 
{
  while(SPISR_SPIF != 1);//wait for data to shift in
  return SPIDR;
}

void spi_flash_shiftout(char x)
{
  while(SPISR_SPTEF != 1);//read the SPTEF bit, continue if bit is 1
  SPIDR = x;                 //write data to SPI data register
//  delay(5);
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
	while(read != '\n' && addr > 0)
	{
		addr--;
		read = spi_flash_read_addr(addr);
	}
	
	if(addr > 0)
		addr++;
	
	spi_flash_read_word(addr, buffer);
}

long get_addr(char letter)
{
	long addresses[27] = {
		0,//a
		0xA889,//b
		0x15C0C,//c
		0x231DC,//d
		0x2B608,//e
		0x30CB7,//f
		0x379AD,//g
		0x3E570,//h
		0x43F20,//i
		0x473DC,//j
		0x497D6,//k
		0x4CBDD,//l
		0x52A52,//m
		0x5AE84,//n
		0x5E46D,//o
		0x62707,//p
		0x6D893,//q
		0x6E5FC,//r
		0x76885,//s
		0x87D47,//t
		0x91566,//u
		0x96208,//v
		0x98D0B,//w
		0x9D584,//x
		0x9D863,//y
		0x9E958,//z
		dict_length
		
	};
	return addresses[letter-'a'];
}

char spi_flash_word_exists(char* word)
{
	long start = get_addr(word[0]);
	long end = get_addr(word[0]+1);
	char buffer[10];
	long addr = start;
	char i;
	while(addr < end)
	{
		spi_flash_read_current_word(addr,buffer);
		if(strcmp(word,buffer)==0)
		{
			return 1;
		}
		//move addr to after next newline
		while(spi_flash_read_addr(addr) != '\n')
			addr++;//moves to next newline
		addr++;//moves after next newline
	}
	return 0;
}