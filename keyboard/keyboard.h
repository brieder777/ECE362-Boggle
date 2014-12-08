/* 
 * File:   keyboard.h
 * Author: Brian Rieder <brieder@purdue.edu>
 *
 * Created on December 6, 2014, 12:13 PM
 * 
 * ============================================================================
 * 
 * This library was made to be compatible with the CodeWarrior compiler.
 */

#ifndef BOARD_H
#define	BOARD_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    INVALID = 0,
    A = 0x1C,
    B = 0x32,
    C = 0x21,
    D = 0x23,
    E = 0x24,
    F = 0x2B,
    G = 0x34,
    H = 0x33,
    I = 0x43,
    J = 0x3B,
    K = 0x42,
    L = 0x4B,
    M = 0x3A,
    N = 0x31,
    O = 0x44,
    P = 0x4D,
    Q = 0x15,
    R = 0x2D,
    S = 0x1B,
    T = 0x2C,
    U = 0x3C,
    V = 0x2A,
    W = 0x1D,
    X = 0x22,
    Y = 0x35,
    Z = 0x1A,
    ENTER = 0x5A,
    BACKSPACE = 0x66,
    SPACE = 0x29,
    ESCAPE = 0x76,
    ONE = 0x16,
    TWO = 0x1E,
    THREE = 0x26, 
    FOUR = 0x25,
    FIVE = 0x2E,
    SIX = 0x36,
    SEVEN = 0x3D,
    EIGHT = 0x3E,
    NINE = 0x46,
    ZERO = 0x45
} ScanCode;

/**
* Interrupt service routine triggered by the keyboard through the IRQ
* pin on the microcontroller.
* 
* Instead of returning a character, this function uses 'keyboard_char_to_buff' to
* write to the global character buffer 'keyboard_char_buff'.
*
*/
interrupt 6 void IRQ_ISR(void);

/**
* Uses PS/2 Scan Code Set 2 to change keyboard input to actual characters.
* 
* Returns the character of the pressed key.
*
*/
unsigned char translate_keyboard_character(ScanCode buff_char);

/**
* Sets registers to allow for keyboard input.
* 
* Instead of returning anything, this function sets specific registers to allow
* manipulation of Port M and intializes interrupts for IRQ.
*
*/
void keyboard_init(void);

/**
 * Get the last ScanCode sent from the keyboard.
 * 
 * The last ScanCode is cleared when this function is accessed.
 * 
 * @return          The last ScanCode if valid, INVALID (0) otherwise.
 */
ScanCode keyboard_getcode(void);

/**
 * Get the last character received from the keyboard.
 * @return          The last character if the last ScanCode was a character,
 *                  0 otherwise.
 */
char keyboard_getchar(void);

/**
 * Intuitive backspace implementation.
 * 
 * Instead of returning, this function acts as a backspace in the normal
 * regard by erasing the previous character.
 */
void backspace_and_remove(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BOARD_H */