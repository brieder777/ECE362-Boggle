/* 
 * File:   boggle.c
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on November 22, 2014, 7:26 PM
 */

#include "boggle.h"
#include <string.h>

/*
 * Implementation Details for `generate_grid`
 * ==========================================
 * From http://everything2.com/title/Boggle, the letter distribution of Boggle
 * dice is (out of 96):
 * 
 * A-6    H-5    O-7    V-2
 * B-2    I-6    P-2    W-3
 * C-2    J-1    Qu-1   X-1
 * D-3    K-1    R-5    Y-3
 * E-11   L-4    S-6    Z-1
 * F-2    M-2    T-9
 * G-2    N-6    U-3
 * 
 * Let's simplify this. Maybe split this into 3 groups?:
 *		Vowels - A, E, I, O, U:		33 / 96
 *		Rare   - J, K, Q, X, Z:		 5 / 96
 *		Common Consonants:			58 / 96
 */
void generate_grid(unsigned int seed)
{
	char i, j;
	char val;
	
	srand(seed); // Seed the random number generator.
	
	for(i = 0; i < BOGGLE_SIZE; i++)
		for(j = 0; j < BOGGLE_SIZE; j++)
		{
			val = rand() % 96;
			
			if(val < 6)
				boggle_grid[i][j] = 'A';
			else if(val < 8)
				boggle_grid[i][j] = 'B';
			else if(val < 10)
				boggle_grid[i][j] = 'C';
			else if(val < 13)
				boggle_grid[i][j] = 'D';
			else if(val < 24)
				boggle_grid[i][j] = 'E';
			else if(val < 26)
				boggle_grid[i][j] = 'F';
			else if(val < 28)
				boggle_grid[i][j] = 'G';
			else if(val < 33)
				boggle_grid[i][j] = 'H';
			else if(val < 39)
				boggle_grid[i][j] = 'I';
			else if(val < 40)
				boggle_grid[i][j] = 'J';
			else if(val < 41)
				boggle_grid[i][j] = 'K';
			else if(val < 45)
				boggle_grid[i][j] = 'L';
			else if(val < 47)
				boggle_grid[i][j] = 'M';
			else if(val < 53)
				boggle_grid[i][j] = 'N';
			else if(val < 60)
				boggle_grid[i][j] = 'O';
			else if(val < 62)
				boggle_grid[i][j] = 'P';
			else if(val < 63)
				boggle_grid[i][j] = 'Q';
			else if(val < 68)
				boggle_grid[i][j] = 'R';
			else if(val < 74)
				boggle_grid[i][j] = 'S';
			else if(val < 83)
				boggle_grid[i][j] = 'T';
			else if(val < 86)
				boggle_grid[i][j] = 'U';
			else if(val < 88)
				boggle_grid[i][j] = 'V';
			else if(val < 91)
				boggle_grid[i][j] = 'W';
			else if(val < 92)
				boggle_grid[i][j] = 'X';
			else if(val < 95)
				boggle_grid[i][j] = 'Y';
			else
				boggle_grid[i][j] = 'Z';
		}
}

// Variable to hold 
static Grid visited = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};

/**
 * Helper function to `validate_word_grid`. Finds out whether a word can be
 * produced from a given starting location (i, j). 
 * 
 * @param word		Rest of word to find
 * @param grid		Temporary grid containing NULL chars. in visited spaces.
 * @param x			Row
 * @param y			Column
 * @return			1 if valid, 0 otherwise.
 */
static char is_adjacent(char* word, char x, char y)
{
	char i, j;
        
        char iter;
	
	// If we've reached the end of the word, it has been found.
	if(*word == '\0')
		return 1;
	
	// Mark current location as visited.
	visited[x][y] = 1;
	
	// Look through all valid surrounding locations (potentially 8).
	for(iter = 0; iter < 8; iter++)
	{
		switch(iter)
		{
			case 0:
				i = x - 1;
				j = y - 1;
				break;
			case 1:
				i = x;
				j = y - 1;
				break;
			case 2:
				i = x + 1;
				j = y - 1;
				break;
			case 3:
				i = x + 1;
				j = y;
				break;
			case 4:
				i = x + 1;
				j = y + 1;
				break;
			case 5:
				i = x;
				j = y + 1;
				break;
			case 6:
				i = x - 1;
				j = y + 1;
				break;
			case 7:
				i = x - 1;
				j = y;
				break;
		}
		
		if(i>=0 && i<BOGGLE_SIZE && j>=0 && j<BOGGLE_SIZE)
		{
			if(*word == boggle_grid[i][j] && !visited[i][j])
			{
				if(is_adjacent(word + 1, i, j))
				{
					// Clear the visited flag and exit.
					visited[x][y] = 0;
					return 1;
				}
			}
		}
	}
	
	// Clear the visited flag and exit.
	visited[x][y] = 0;
	return 0;
}

char validate_word_grid(char* word)
{
	char i, j;

	// Look for possible first letters.
	for(i = 0; i < BOGGLE_SIZE; i++)
		for(j = 0; j < BOGGLE_SIZE; j++)
			if(*word == boggle_grid[i][j])
			{
				// Find possible second, third, etc. letters
				if(is_adjacent(word + 1, i, j))
					return 1;
			}
	
	return 0;
}

char calculate_points(char* word)
{
	char points = 0;
	char itr = 0;

	while(word[itr] != '\0')
		++itr;

	switch(itr) {
		case 3:
			points = 1;
			break;
		case 4:
			points = 1;
			break;
		case 5:
			points = 2;
			break;
		case 6:
			points = 3;
			break;
		case 7:
			points = 5;
			break;
		case 8:
			points = 11;
			break;
		default:
			points = 0;
	}

	return points;
}


void Player_init(Player* player)
{
	player->word_count = 0;
}

void Player_add_word(Player* player, char* word)
{
	strcpy(player->prev_words[player->word_count], word);
	
	(player->word_count)++;
}
