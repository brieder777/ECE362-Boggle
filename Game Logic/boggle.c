/* 
 * File:   boggle.c
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on November 22, 2014, 7:26 PM
 */

#include "boggle.h"

void generate_grid(unsigned int seed)
{
	char i, j;
	
	srand(seed); // Seed the included random number generator.
	
	for(i = 0; i < BOGGLE_SIZE; i++)
		for(j = 0; j < BOGGLE_SIZE; j++)
		{
			
		}
}

/**
 * Helper function to `validate_word_grid`. Finds out whether a word can be
 * produced from a given starting location (i, j). 
 * 
 * @param word		Rest of word to find
 * @param grid		Temporary grid containing NULL chars. in visited spaces.
 * @param i			Row
 * @param j			Column
 * @return			1 if valid, 0 otherwise.
 */
static char is_adjacent(char* word, char grid[BOGGLE_SIZE][BOGGLE_SIZE],
		char i, char j)
{
	// If we've reached the end of the word, it hasn't been found.
	if(*word == '\0')
		return 0;
	
	// Mark current location as visited.
	grid[i, j] = '\0';
	
	// Look through all valid surrounding locations (potentially 8).
	
}

char validate_word_grid(char* word)
{
	char i, j;
	char temp_grid[BOGGLE_SIZE][BOGGLE_SIZE];
	
	// Copy the grid to temp_grid.
	for(i = 0; i < BOGGLE_SIZE; i++)
		for(j = 0; j < BOGGLE_SIZE; j++)
			temp_grid[i][j] = boggle_grid[i][j];
	
	// Look for possible first letters.
	for(i = 0; i < BOGGLE_SIZE; i++)
		for(j = 0; j < BOGGLE_SIZE; j++)
			if(*word == temp_grid[i][j])
			{
				// Recurse to find possible second, third, etc. letters
				if(is_adjacent(word + 1, temp_grid, i, j))
					return 1;
			}
	
	return 0;
}

char calculate_points(char* word)
{
	char points = 0;
	
	
	return points;
}