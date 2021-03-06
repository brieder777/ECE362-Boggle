/*
***********************************************************************                       
  IRQ triggered interrupt service routine       
***********************************************************************
*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>
#include "keyboard.h"

// Last scan code that was read -- needs to be translated
static unsigned char keyboard_last_code;
static unsigned char keyboard_last_code_x;

  // Initialize keyboard frame
  static union {
    int word;
    struct {
      unsigned char high;
      unsigned char low;
    } bytes;
  } keyboard_frame;

// Initialize keyboard frame
  static union {
    int word;
    struct {
      unsigned char high;
      unsigned char low;
    } bytes;
  } keyboard_frame_x;


  // Initialize local variables
  static unsigned char count = 0;
  static unsigned char count_x = 0;
  static unsigned char breakflag = 0;
  static unsigned char breakflag_x = 0;

interrupt 6 void IRQ_ISR(void)
{
  // Send clear signal to flip-flop -- high, low, high
  PTM_PTM1 = 1;
  PTM_PTM1 = 0;
  PTM_PTM1 = 1;

  // Shift in only the bytes between start and parity
  if(count >= 1 && count <= 8)
  {
    keyboard_frame.bytes.high = PTM_PTM0;
    keyboard_frame.word = keyboard_frame.word >> 1;
  }

  // After 11 bits have been read, send the character out
  if(count == 10)
  {
    if(breakflag==1)
      breakflag = 0;
    else if(keyboard_frame.bytes.low == 0xF0)
      breakflag = 1;
    else
      keyboard_last_code = keyboard_frame.bytes.low;
  } 
  
  // Increment number of bits read with modulus
  count = (count + 1) % 11;
}

interrupt 5 void XIRQ_ISR(void)
{
  // Send clear signal to flip-flop -- high, low, high
  PTT_PTT1 = 1;
  PTT_PTT1 = 0;
  PTT_PTT1 = 1;

  // Shift in only the bytes between start and parity
  if(count_x >= 1 && count_x <= 8)
  {
    keyboard_frame_x.bytes.high = PTT_PTT0;
    keyboard_frame_x.word = keyboard_frame_x.word >> 1;
  }

  // After 11 bits have been read, send the character out
  if(count_x == 10)
  {
    if(breakflag_x==1)
      breakflag_x = 0;
    else if(keyboard_frame_x.bytes.low == 0xF0)
      breakflag_x = 1;
    else
      keyboard_last_code_x = keyboard_frame_x.bytes.low;
  } 
  
  // Increment number of bits read with modulus
  count_x = (count_x + 1) % 11;
}

void keyboard_init(void)
{
  // Initialize the Port M 0 and 1 as input and output respectively
  DDRM_DDRM0 = 0; // Data received from keyboard 1
  DDRM_DDRM1 = 1; // CLR signal sent to flip-flop 1

  DDRT_DDRT0 = 0; // Data received from keyboard 2
  DDRT_DDRT1 = 1; // CLR signal sent to flip-flop 2

  // Initialize interrupts
  INTCR_IRQE = 0;
  INTCR_IRQEN = 1;
  PTM_PTM1 = 1;
  PTT_PTT1 = 1;

  asm {
    andcc #$BF
  }
}

ScanCode keyboard_getcode()
{
	ScanCode code = keyboard_last_code;
	keyboard_last_code = 0;
	return code;
}

ScanCode keyboard_getcode_x()
{
  ScanCode code = keyboard_last_code_x;
  keyboard_last_code_x = 0;
  return code;
}

char keyboard_getchar()
{
	ScanCode code;
	
	code = keyboard_getcode();
	if(code)
		return translate_keyboard_character(code);
	else
		return 0;
}

unsigned char translate_keyboard_character(ScanCode buff_char)
{
  switch(buff_char) {
    case A:
      return 'A';
    case B:
      return 'B';
    case C:
      return 'C';
    case D:
      return 'D';
    case E:
      return 'E';
    case F:
      return 'F';
    case G:
      return 'G';
    case H:
      return 'H';
    case I:
      return 'I';
    case J:
      return 'J';
    case K:
      return 'K';
    case L:
      return 'L';
    case M:
      return 'M';
    case N:
      return 'N';
    case O:
      return 'O';
    case P:
      return 'P';
    case Q:
      return 'Q';
    case R:
      return 'R';
    case S:
      return 'S';
    case T:
      return 'T';
    case U:
      return 'U';
    case V:
      return 'V';
    case W:
      return 'W';
    case X:
      return 'X';
    case Y:
      return 'Y';
    case Z:
      return 'Z';
    case ENTER:
      return '\n';
    case SPACE:
      return ' ';
    case BACKSPACE:
      return '\b';
    case ONE:
      return '1';
    case TWO:
      return '2';
    case THREE:
      return '3';
    case FOUR:
      return '4';
    case FIVE:
      return '5';
    case SIX:
      return '6';
    case SEVEN:
      return '7';
    case EIGHT:
      return '8';
    case NINE:
      return '9';
    case ZERO:
      return '0';
    default:
      return '\0';
  }
}