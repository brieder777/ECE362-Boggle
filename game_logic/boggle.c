/* 
 * File:   boggle.c
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on November 22, 2014, 7:26 PM
 */

#include "boggle.h"

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
	
//	srand(seed); // Seed the random number generator.
	
	for(i = 0; i < BOGGLE_SIZE; i++)
		for(j = 0; j < BOGGLE_SIZE; j++)
		{
			
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
        for(iter = 0 ; iter < 8 ; iter++)
        {
            if(iter == 0)
            {
                i=x-1;
                j=y-1;
            }else if(iter == 1)
            {
                i=x;
                j=y-1;
            }else if(iter == 2)
            {
                i=x+1;
                j=y-1;
            }else if(iter == 3)
            {
                i=x+1;
                j=y;
            }else if(iter == 4)
            {
                i=x+1;
                j=y+1;
            }else if(iter == 5)
            {
                i=x;
                j=y+1;
            }else if(iter == 6)
            {
                i=x-1;
                j=y+1;
            }else//iter== 7
            {
                i=x-1;
                j=y;
            }        
            if(i>=0 && i<BOGGLE_SIZE && j>=0 && j<BOGGLE_SIZE
               && *word == boggle_grid[i][j] 
               && !visited[x][y]
               && is_adjacent(word + 1, i, j))
            {
                    // Clear the visited flag and exit.
                    visited[x][y] = 0;
                    return 1;
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
	
	
	return points;
}