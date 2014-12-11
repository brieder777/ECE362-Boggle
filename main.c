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
#include "display/lcd.h"


#pragma MESSAGE DISABLE C2705 // Possible loss of data
#pragma MESSAGE DISABLE C5909 // Assignment in condition
#pragma MESSAGE DISABLE C4000 // Condition always TRUE

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
char flasher = 0;   // flash for pwm

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

	// Initialize pwm
	DDRT_DDRT2 = 1;
	MODRR = 0x04;
	PWME = 0x04;
	PWMPOL = 0x04;
	PWMCTL = 0x00;
	PWMCAE = 0x00;
	PWMPER2 = 0xFF;
	PWMDTY2 = 0x00;
	PWMCLK = 0x00;
	PWMPRCLK = 0x01;

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
	lcdinit();
	keyboard_init();
	EnableInterrupts;
	
	for(;;)
	{
		// Clear all displays.
		lcd_send_i(LCDCLR, BIG_LCD);
		lcd_send_i(LCDCLR, SMALL_LCD_1);
		PWMDTY2 = 0;
		
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
			
			default:
				main_menu_entry();
				break;
		}
	} /* loop forever */

} /* do not leave main */

void main_menu_entry()
{
	ScanCode code;
	
	lcd_chgline(LINE1,BIG_LCD);

	lcd_message("   ECE 362 Boggle   ", BIG_LCD);
	
	lcd_chgline(LINE3, BIG_LCD);
	lcd_message("1. Start Random Game", BIG_LCD);
	
	lcd_chgline(LINE4, BIG_LCD);
	lcd_message("2. Enter Seed", BIG_LCD);
	
	while(1)
	{
		while((code = keyboard_getcode()) == 0);
		

		if(code == ONE)
		{
			lcd_print_c('1',SMALL_LCD_1);
			screen = RANDOM;
			break;
		}
		else if(code == TWO)
		{
			lcd_print_c('2',SMALL_LCD_1);
			screen = SEED;
			break;
		}
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
	generate_grid((TCNT == 0) ? 1 : TCNT);
	
	screen = GAME;
}

void seed_entry()
{
    int seed = 0;
    char keypress;
	
	lcd_chgline(LINE1,BIG_LCD);
    lcd_message("Enter a seed: ",BIG_LCD);
	lcd_chgline(LINE2,BIG_LCD);
    while ((keypress = keyboard_getchar()) == 0);
    while (keypress != '\n') {
        seed *= 10;
        seed += keypress - '0';
		lcd_print_c(keypress,BIG_LCD);
        while((keypress = keyboard_getchar()) == 0);
    }
    
	if(seed == 0)
		seed = 1;
	
	generate_grid(seed);
    
    screen = GAME;
}

void game_entry()
{
	char time_string[4];
	// Display grid
	int i;
	int j;
	int tim_rem = 120;
//	int tim_rem = 10;
	ScanCode keypress=0;
	char entered_wordlen = 0;
	
	char buffer[BOGGLE_WORDLEN+1];

	lcd_chgline(LINE1,BIG_LCD);
	// Display grid.
	for(i = 0; i < BOGGLE_SIZE; i++)
	{
		for(j = 0; j < BOGGLE_SIZE; j++)
		{
			lcd_print_c(boggle_grid[i][j],BIG_LCD);
			lcd_print_c(' ',BIG_LCD);
		}
		switch(i){
			case 0:
				lcd_chgline(LINE2,BIG_LCD);
				break;
			case 1:
				lcd_chgline(LINE3,BIG_LCD);
				break;
			case 2:
				lcd_chgline(LINE4,BIG_LCD);
				break;
		}
	}

	lcd_chgline(LINE2 + 10,BIG_LCD);
	lcd_message("Time: ",BIG_LCD);
	while(tim_rem != 0)
	{
		// Output time remaining.
		if(second == 1) {
			second = 0;
			tim_rem -= 1;
			sprintf(time_string, "%3d", tim_rem);
			
			lcd_chgline(LINE2 + 16, BIG_LCD);
			lcd_message(time_string, BIG_LCD);
			PWMDTY2 = 255 - tim_rem*255/120;

		}
		if(tim_rem<=10){
			PWMDTY2 = flasher*255;
		}
		
		// Look for keyboard input.
		
			
		keypress = keyboard_getcode_x();
		//keypress_x = keyboard_getcode_x();
		//@TODO: Add second player functionality
		if(keypress != 0)
		{
			
			
			if(keypress == BACKSPACE)
			{
				if(entered_wordlen > 0)
				{
					lcd_backspace();
					entered_wordlen--;
				}
			}
			else if(keypress == ENTER)
			{
				if(entered_wordlen > 2)
				{
					buffer[entered_wordlen]= '\0';
					while(entered_wordlen > 0)
					{
						lcd_backspace();
						entered_wordlen--;	
					}
					//perform validation of word in buffer
					// @todo make this a function
					if(validate_word_grid(buffer))
					{
						lcd_message("Maybe Yaaaaay!!!!",SMALL_LCD_1);
					}
					else
					{
						lcd_message("Not yay",SMALL_LCD_1);
					}
				}
			}
			else if(keypress == ESCAPE)
			{
				screen = MAIN_MENU;
				return;
			}
			else if(entered_wordlen < BOGGLE_WORDLEN)
			{
				buffer[entered_wordlen] = translate_keyboard_character(keypress);
				lcd_print_c(buffer[entered_wordlen],SMALL_LCD_1);
				entered_wordlen++;
			}
		}	
	}
	screen = GAME_OVER;
}

void game_over_entry()
{
	ScanCode keypress;
	
	outstr("\n\rGame Over!\n\r");
	outstr("Press ENTER to\n\r");
	outstr("play again!\n\r");
	
	while ((keypress = keyboard_getcode()) != ENTER);
	
	screen = MAIN_MENU;
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
	if(tim_cnt % 25 == 0)
	{
		flasher = !flasher;
	}
	
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