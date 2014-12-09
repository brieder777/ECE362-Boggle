/* 
 * File:   spi_flash.h
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on December 4, 2014, 6:41 PM
 * 
 * ============================================================================
 * 
 * Driver for the W25Q80BV 1MB SPI Flash chip.
 */

#ifndef SPI_FLASH_H
#define	SPI_FLASH_H

#ifdef	__cplusplus
extern "C" {
#endif

// Instruction Characters
#define spi_flash_read 0x03
    
#define CS_PIN PTT_PTT3
#define dict_length 653339

union Address{
  long addr;
  char bytes[4];
};




void spi_flash_shiftout(char x);
char spi_flash_shiftin(void);
void delay(unsigned int x);
char spi_flash_read_addr(long addr);
void spi_flash_read_word(long addr, char* buffer);
void spi_flash_read_current_word(long addr, char* buffer);

void spi_flash_word_exists(char* word);


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_FLASH_H */

