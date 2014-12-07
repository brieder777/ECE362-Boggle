/* 
 * File:   termio.h
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on October 31, 2014, 11:28 AM
 */

#ifndef TERMIO_H
#define	TERMIO_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
 ***********************************************************************
 Character I/O Library Routines for 9S12C32 (for debugging only)
 ***********************************************************************
 Name:         inchar
 Description:  inputs ASCII character from SCI serial port and returns it
 Example:      char ch1 = inchar();
 ***********************************************************************
 */
char inchar(void);

/*
 ***********************************************************************
 Name:         outchar
 Description:  outputs ASCII character x to SCI serial port
 Example:      outchar('x');
 ***********************************************************************
 */
void outchar(char x);

/**
 * General purpose debugging function that writes a character in binary
 * to the SCI serial port.
 *
 */
void outbin(char x);

/**
 * General purpose debugging function that writes an integer in binary
 * to the SCI serial port.
 *
 */
void outbin_int(int x);

/**
 * Output a string to an SCI terminal.
 * @param str
 */
void outstr(char* str);

/**
 * Output a number to an SCI terminal.
 * @param number
 * @param digits
 */
void outnum(unsigned int number, unsigned char digits);

#ifdef	__cplusplus
}
#endif

#endif	/* TERMIO_H */

