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

#define BOGGLE_SIZE 4
#define BOGGLE_WORDLEN 7
#define BOGGLE_MAXWORDS 25

/* 
 * Information about a player.
 * 
 * This struct currently uses 401 bytes of memory.
 * 
 * @TODO We could potentially make this more space-effecient by putting all the
 * words in one contiguous array, and delimiting them by null characters.
 */
typedef struct boggle_player {
    char word_count; // Number of words found.
    char prev_words[BOGGLE_MAXWORDS][BOGGLE_WORDLEN + 1]; // Array of previously found words.
    unsigned char score; // Score. (Sum of points of correct words.)
} Player;

typedef char Grid[BOGGLE_SIZE][BOGGLE_SIZE];

// Global letter grid variable.
//
// If we want to be able to use the letters QU, we would need to change this
// structure.
extern Grid boggle_grid;



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
char validate_word_prev(char* word, Player* player);

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
char validate_word_grid(char* word);

/**
 * Calculate the number of points that a word is worth.
 * 
 * This does not validate the correctness of the word.
 * 
 * By Boggle rules, scoring works as follows:
 * <table>
 *  <tr><th> No. of Letters </th>    <th> Points </th></tr>
 *  <tr><td>              3 </td>    <td>      1 </td></tr>
 *  <tr><td>              4 </td>    <td>      1 </td></tr>
 *  <tr><td>              5 </td>    <td>      2 </td></tr>
 *  <tr><td>              6 </td>    <td>      3 </td></tr>
 *  <tr><td>              7 </td>    <td>      5 </td></tr>
 *  <tr><td>             8+ </td>    <td>     11 </td></tr>
 * </table>
 * 
 * @param word      A string containing the word. (Terminated with a NULL
 *                  character.)
 * @return          The number of points the word is worth.
 */
char calculate_points(char* word);

/**
 * Initialize a Player object. It must be constructed externally.
 * 
 * @param player    The player object to initialize.
 */
void Player_init(Player* player);

/**
 * Add a word to a player's list of previous words, and add the points for the
 * word.
 * 
 * @param player    The player whose list to add the word to.
 * @param word      A string containing the word to add. (Terminated with a
 *                  NULL character.)
 */
void Player_add_word(Player* player, char* word);


#ifdef	__cplusplus
}
#endif

#endif	/* BOGGLE_H */

