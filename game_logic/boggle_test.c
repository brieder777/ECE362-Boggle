/* 
 * File:   boggle_test.c
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on November 30, 2014, 3:35 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "boggle.h"

/*
 * 
 */
int main(int argc, char** argv)
{
//	Grid grid = {{'A','B','C','D'},
//				  {'E','F','G','H'},
//				  {'I','J','K','L'},
//				  {'M','N','O','P'}};
	boggle_grid[0][0] = 'A';
	boggle_grid[0][1] = 'B';
	boggle_grid[0][2] = 'C';
	boggle_grid[0][3] = 'D';
	boggle_grid[1][0] = 'E';
	boggle_grid[1][1] = 'F';
	boggle_grid[1][2] = 'G';
	boggle_grid[1][3] = 'H';
	boggle_grid[2][0] = 'I';
	boggle_grid[2][1] = 'J';
	boggle_grid[2][2] = 'K';
	boggle_grid[2][3] = 'L';
	boggle_grid[3][0] = 'M';
	boggle_grid[3][1] = 'N';
	boggle_grid[3][2] = 'O';
	boggle_grid[3][3] = 'P';
	
	char * word = "AFKG";
	printf(word);
	if(validate_word_grid(word))
	{
		printf("yes");
	}
	else
	{
		printf("nope");
	}

	return(EXIT_SUCCESS);
}

