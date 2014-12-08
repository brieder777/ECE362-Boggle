/*
 ************************************************************************
 ECE 362 - Mini-Project C Source File - Fall 2014
 ***********************************************************************
	 	   			 		  			 		  		
 Team ID: 29

 Project Name: Boggle

 Team Members:

   - Team/Doc Leader: < ? >      Signature: ______________________
   
   - Software Leader: < ? >      Signature: ______________________

   - Interface Leader: < ? >     Signature: ______________________

   - Peripheral Leader: < ? >    Signature: ______________________


 Academic Honesty Statement:  In signing above, we hereby certify that we 
 are the individuals who created this HC(S)12 source file and that we have
 not copied the work of any other student (past or present) while completing 
 it. We understand that if we fail to honor this agreement, we will receive 
 a grade of ZERO and be subject to possible disciplinary action.

 ***********************************************************************

 The objective of this Mini-Project is to .... < ? >


 ***********************************************************************

 List of project-specific success criteria (functionality that will be
 demonstrated):

 1.

 2.

 3.

 4.

 5.

 ***********************************************************************

  Date code started: 11/22/2014

  Update history (add an entry every time a significant change is made):

  Date: < ? >  Name: < ? >   Update: < ? >

  Date: < ? >  Name: < ? >   Update: < ? >

  Date: < ? >  Name: < ? >   Update: < ? >


 ***********************************************************************
 */

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

#include <stdio.h>
#include "game_logic/boggle.h"
#include "keyboard/keyboard.h"
#include "course_includes/our_termio.h"

/* All functions after main should be initialized here */
void main_menu_entry(void);
void high_scores_entry(void);
void random_entry(void);
void seed_entry(void);
void game_entry(void);
void game_over_entry(void);


/* Variable declarations */

// Current screen / mode of operation.
enum {
	MAIN_MENU,
	HIGH_SCORES,
	RANDOM,
	SEED,
	GAME,
	GAME_OVER
} screen = MAIN_MENU;

int tim_cnt = 0;	// Hundredth-second counter
char second = 0;	// One-second flag

/* Special ASCII characters */
#ifndef CR
	#define CR 0x0D		// ASCII return
#endif
#ifndef LF
	#define LF 0x0A		// ASCII new line
#endif

/*	 	   		
 ***********************************************************************
 Initializations
 ***********************************************************************
 */

void initializations(void)
{

	/* Set the PLL speed (bus clock = 24 MHz) */
	CLKSEL = CLKSEL & 0x80; //; disengage PLL from system
	PLLCTL = PLLCTL | 0x40; //; turn on PLL
	SYNR = 0x02; //; set PLL multiplier
	REFDV = 0; //; set PLL divider
	while(!(CRGFLG & 0x08))
	{
	}
	CLKSEL = CLKSEL | 0x80; //; engage PLL

	/* Disable watchdog timer (COPCTL register) */
	COPCTL = 0x40; //COP off; RTI and COP stopped in BDM-mode

	/* Initialize asynchronous serial port (SCI) for 9600 baud, interrupts off initially */
	SCIBDH = 0x00; //set baud rate to 9600
	SCIBDL = 0x9C; //24,000,000 / 16 / 156 = 9600 (approx)  
	SCICR1 = 0x00; //$9C = 156
	SCICR2 = 0x0C; //initialize SCI for program-driven operation
	DDRB = 0x10; //set PB4 for output mode
	PORTB = 0x10; //assert DTR pin on COM port

	/* Initialize peripherals */
	
	// Initialize TIM Ch 7 for periodic interrupts at 100 Hz
	TSCR1_TEN = 1;	// Enable timer subsystem
	TIOS_IOS7 = 1;	// Set Ch 7 to output compare mode
	TSCR2_PR = 7;	// Prescale clock to 24 MHz / 2^7 = 187.5 KHz 
	TSCR2_TCRE = 1;	// Reset after OC7
	TC7 = 1875;		// Scale clock to 187.5 KHz / 1875 = 100 Hz
	TIE_C7I = 1;	// Enable interrupts

	/* Initialize interrupts */


}

/*	 		  			 		  		
 ***********************************************************************
Main
 ***********************************************************************
 */
void main(void)
{
	DisableInterrupts
	initializations();
	keyboard_init();
	EnableInterrupts;

	for(;;)
	{
		// Clear the main display.
		outstr("\033[H\033[2J");
		
		// Entry point to to parts of program.
		switch(screen)
		{
			case MAIN_MENU:
				main_menu_entry();
				break;
				
			case RANDOM:
				random_entry();
				break;
				
			case SEED:
				seed_entry();
				break;
				
			case HIGH_SCORES:
				high_scores_entry();
				break;
				
			case GAME:
				game_entry();
				break;
				
			case GAME_OVER:
				game_over_entry();
				break;
		}
	} /* loop forever */

} /* do not leave main */

void main_menu_entry()
{
	ScanCode code;
	
	outstr("   ECE 362 Boggle   \r\n");
	outstr("\r\n");
	outstr("1. Start Random Game\r\n");
	outstr("2. Enter Seed");
	
	while(1)
	{
		while((code = keyboard_getcode()) == 0);

		if(code == ONE)
		{
			screen = RANDOM;
			break;
		}
		else if(code == TWO)
		{
			screen = SEED;
			break;
		}
		outstr("Problem");
	}
}

void high_scores_entry()
{
	while(1)
	{
		
	}
}

void random_entry()
{
	outstr("Generating Game\r\n");
	outstr("Please Wait...\r\n");
	
	generate_grid(TCNT);
	
	outnum(TCNT, 5); // (Debug the random number)
	
	screen = GAME;
}

void seed_entry()
{
    int seed = 0;
    char keypress;
	
    outstr("Enter a seed:\r\n");
    while ((keypress = keyboard_getchar()) == 0);
    while (keypress != '\n') {
        seed *= 10;
        seed += keypress - '0';
		outchar(keypress);
        while((keypress = keyboard_getchar()) == 0);
    }
    outstr("Generating Game\r\n");
    outstr("Please Wait...\r\n");
    generate_grid(seed);
    
    outnum(seed, 5);
    
    screen = GAME;
}

void game_entry()
{
	// Display grid
	int i;
	int j;
	int tim_rem = 120;
	for(i = 0; i < BOGGLE_SIZE; i++)
	{
		outstr("\r\n");

		for(j = 0; j < BOGGLE_SIZE; j++)
		{
			outchar(boggle_grid[i][j]);
			outchar(' ');
		}
	}

	boggle_grid;
	outchar('\n');
	outstr("Time Remaining:\n\r");
	while(tim_rem != 0)
	{
		if(second == 1) {
			second = 0;
			tim_rem -= 1;
			outnum(tim_rem, 3);
			outstr("\b\b\b");
		}
	}
	screen = GAME_OVER;
}

void game_over_entry()
{
	outstr("\n\rGame Over!");
	while(1)
	{
		
	}
}


/*
 ************************************************************************
 RTI interrupt service routine: RTI_ISR
 ************************************************************************
 */
interrupt 7 void RTI_ISR(void)
{
	// clear RTI interrupt flag
	CRGFLG = CRGFLG | 0x80;


}

/*
 ***********************************************************************
  TIM interrupt service routine	  		
 ***********************************************************************
 */
interrupt 15 void TIM_ISR(void)
{
	// clear TIM CH 7 interrupt flag 
	TFLG1 = TFLG1 | 0x80;

	tim_cnt = (tim_cnt + 1) % 100;
	
	if(tim_cnt == 0)
		second = 1;
}

/*
 ***********************************************************************
  SCI interrupt service routine		 		  		
 ***********************************************************************
 */
interrupt 20 void SCI_ISR(void)
{



}