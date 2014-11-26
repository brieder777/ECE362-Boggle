/* 
 * File:   boggle.h
 * Author: Subhav Ramachandran <subhav@purdue.edu>
 *
 * Created on November 22, 2014, 7:26 PM
 * 
 * ============================================================================
 * 
 * To make testing easier, this library should be compatible with both GCC and
 * CodeWarrior compilers.
 */

#ifndef BOGGLE_H
#define	BOGGLE_H

#ifdef	__cplusplus
extern "C" {
#endif

// Global letter grid variable.
//
// If we want to be able to use the letters QU, we would need to change this
// structure.
char boggle_grid[4][4];



/**
 * Generate a new letter grid.
 * 
 * Instead of returning the grid, this function sets the global variable
 * `boggle_grid`.
 *
 * @param seed      A random number used to generate the letter grid. This can
 *                  be generated, for example, by counting RTI interrupts until
 *                  the game begins.
 */
void generate_grid(unsigned int seed);

/**
 * Validate a word against previous correct words.
 * 
 * @param word      A string containing the word to validate. (Terminated with
 *                  a NULL character.)
 * @return          1 if valid, 0 otherwise
 */
int validate_word_prev(char* word);

/**
 * Validate an inputted word against the letter grid.
 * 
 * Validating the word against a dictionary should be done externally from this
 * library.
 * 
 * @param word      A string containing the word to validate. (Terminated with
 *                  a NULL character.)
 * @return          1 if valid, 0 otherwise
 */
int validate_word_grid(char* word);

/**
 * Calculate the number of points that a word is worth.
 * 
 * This does not validate the correctness of the word.
 * 
 * @param word      A string containing the word. (Terminated with a NULL
 *                  character.)
 * @return          The number of points the word is worth.
 */
char calculate_points(char* word);


#ifdef	__cplusplus
}
#endif

#endif	/* BOGGLE_H */

