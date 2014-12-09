#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>
#include "lcd.h"

void lcd_shiftout(char x)
{
  PTAD = x;
}

void lcd_clock(char lcd) 
{ //pulse LCD clock line low->high->low for appropriate lcd
  if(lcd == 1) {
    LCDCLK1 = 0;
    LCDCLK1 = 1;
    LCDCLK1 = 0;
  } else if(lcd == 2) {
    LCDCLK2 = 0;
    LCDCLK2 = 1;
    LCDCLK2 = 0;
  } else if(lcd == 3){
    LCDCLK2 = 0;
    LCDCLK2 = 1;
    LCDCLK2 = 0;
  }
}

void lcdwait()
{  //wait for lcd
  delay(200);
}

void delay(unsigned int x) {
  int i;
  for(i=0;i<x;i++);
}

void lcd_send_byte(char x, char lcd)
{
  lcd_shiftout(x);   // shift out character
  lcd_clock(lcd); // pulse LCD clock line low->high->low
  lcdwait();     // wait 2 ms for LCD to process data
}

void lcd_send_i(char x, char lcd)
{
  RS = 0;   // set the register select line low (instruction data)
  lcd_send_byte(x,lcd);   // send byte
}

void lcd_chgline(char pos, char lcd)
{
  lcd_send_i(0xFE,lcd); //send CURMOV instruction ($FE)
  lcd_send_byte(pos, lcd);
}
 
void lcd_print_c(char x, char lcd)
{
  RS = 1;   // set the register select line high (byte data)
  lcd_send_byte(x,lcd);   // send byte
}

void lcd_message(char x[], char lcd)
{
  int i;
  for(i=0;x[i]!='\0';i++) { //iterate until end of string
    lcd_print_c(x[i],lcd);         //print each character
  }  
}

void lcdinit() {
  DDRT_DDRT4 = 1;//pt4 as output(/cs)  
  DDRT_DDRT5 = 1;//pt5 as output(clk1)   
  DDRT_DDRT6 = 1;//pt6 as output(clk2)   
  DDRT_DDRT7 = 1;//pt7 as output(clk3)   
  RS = 1;//initially /RS is negated
  DDRAD= 0xFF;//configure ptad as output 

  LCDCLK1 = 1;                    //pull LCDCLK high (idle)
  LCDCLK2 = 1; 
  LCDCLK3 = 1; 
  lcd_send_i(LCDON,1);            //turn on LCD (LCDON instruction) ($0F)
  lcd_send_i(LCDON,2);            //turn on LCD (LCDON instruction) ($0F)
  lcd_send_i(TWOLINE,1);          //enable two-line mode (TWOLINE instruction) ($38)
  lcd_send_i(TWOLINE,2);          //enable two-line mode (TWOLINE instruction) ($38)
  lcd_send_i(LCDCLR,1);           //clear LCD (LCDCLR instruction) ($01) 
  lcd_send_i(LCDCLR,2);           //clear LCD (LCDCLR instruction) ($01)
  delay(8000);                    //wait for 2ms so that the LCD can wake up  
}