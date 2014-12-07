/* 
 * File:   termio.c
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on October 31, 2014, 11:30 AM
 */

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

#include "termio.h"

char inchar(void)
{
	/* receives character from the terminal channel */
	while(!(SCISR1 & 0x20)); /* wait for input */
	return SCIDRL;
}

void outchar(char x)
{
	/* sends a character to the terminal channel */
	while(!(SCISR1 & 0x80)); /* wait for output buffer empty */
	SCIDRL = x;
}

void outbin_int(int x)
{
  unsigned char itr;
  for(itr = 0; itr < 16; ++itr) {
    outchar((x & 0x01) + '0');
    x = x >> 1;
  }
  outchar('\r');
  outchar('\n');
}

void outbin(char x)
{
  unsigned char itr;
  for(itr = 0; itr < 8; ++itr) {
    outchar((x & 0x01) + '0');
    x = x >> 1;
  }
  outchar('\r');
  outchar('\n');
}