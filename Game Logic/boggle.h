/* 
 * File:   boggle.h
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on November 22, 2014, 7:26 PM
 */

#ifndef BOGGLE_H
#define	BOGGLE_H

#ifdef	__cplusplus
extern "C" {
#endif

// Global letter grid variable.
// If we want to be able to use the letters QU, we would need to change this
// structure.
char boggle_grid[4][4];



/**
 * Generate a new letter grid.
 * 
 * Instead of returning the grid, this function sets the global variable
 * `boggle_grid`.
 */
void generate_grid(void);

/**
 * Validate an inputted word.
 * 
 * @param word The word to validate.
 * @return 1 if valid, 0 otherwise
 */
int validate_word(char* word);

/**
 * Validate a word against the letter grid.
 * 
 * @param word The word to validate.
 * @return 1 if valid, 0 otherwise
 */
int validate_word_grid(char* word);

/**
 * Validate a word against the dictionary.
 * 
 * @param word The word to validate.
 * @return 1 if valid, 0 otherwise
 */
int validate_word_dict(char* word);


#ifdef	__cplusplus
}
#endif

#endif	/* BOGGLE_H */

