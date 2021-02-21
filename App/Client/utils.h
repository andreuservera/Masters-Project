#ifndef UTILS_H
#define UTILS_H


#include <stdio.h>


#define TRUE 1
#define FALSE 0



/** @file utils.h
 * 
 * @brief Functions to be used in ReadFile. Mainly written by Toni.       
 * 
 */ 



/**************************************************************************/
/** @name getWord *********************************************************/
/**************************************************************************/
//@{

/**
* @brief Read a word of a file a gives its length, if its the end
* of the line the length of the word is 0
*
* @param [FILE *file_pointer] pointer to read the file
*
* @param [char *word] address where the word will be stored, it MUST have enough
* space
*
* @param [int *word_length] address where the length of the word is stored
*
*/

void * getWord(FILE * file_pointer,char *word, int *word_length, int verbosity);



/**************************************************************************/
/** @name compareWords ****************************************************/
/**************************************************************************/
//@{

/**
 * @brief Compare two words(strings)
 *
 * @param [char *wordA] pointer to the first word
 *include
 * @param [int lengthA] number of characters contained in wordA
 *
 * @param [char *wordB] pointer to the second word
 *
 * @param [int lengthB] number of characters contained in wordB
 *
 * @return [int] If both words are the same return TRUE(1) and if not, returns FALSE(0)
 *
 */
int compareWords(char *wordA, int lengthA, char *wordB, int lengthB, int verbosity);



/**************************************************************************/
/** @name copyCharArray ***************************************************/
/**************************************************************************/
//@{

/**
* @brief Copies a char array in another address
*
* @param [char *pointer] pointer to the list of characters
*
* @param [ int array_length] number of characters to be copied from the array
*
 * @return [int] An address containing the copied characters
*
*/

char * copyCharArray(char *pointer, int array_length);



/**************************************************************************/
/** @name BinCharToInt ****************************************************/
/**************************************************************************/
//@{

/**
 * @brief Convert a binary number, given in string format, to decimal
 *
 * @param [char *pointer] pointer to the list of characters
 *
 * @param [ int length] number of characters
 *
 * @return [int] Decimal value
 *
 */

int BinCharToInt (char *pointer, int length);



/**************************************************************************/
/** @name readnextWord ****************************************************/
/**************************************************************************/
char * readnextWord();


/**************************************************************************/
/** @name charToInt *******************************************************/
/**************************************************************************/
int charToInt(char c);


#endif // UTILS_H
