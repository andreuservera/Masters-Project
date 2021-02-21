#include <math.h>
#include <stdlib.h>
#include "utils.h"


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

void * getWord(FILE * file_pointer,char *word, int *word_length, int verbosity)
{
	char c;
	int blanks=0;
	word_length[0] = 0;
	
	if (verbosity == 1)
	{
		printf("[WORD]: ");
	}

	while((c = fgetc(file_pointer)) != EOF)
	{
		if((c == ' ' || c == '\n') || (c == '\t' || c == ','))
		{
			if(word_length[0] != 0)
			{
				break;
			}
		}
		else
		{
			if(verbosity == 1)
			{
				printf("%c",c);
	      	}

	    word[word_length[0]] = c;
		word_length[0]++;
	  	}
	}

	if (c == EOF){
		if(verbosity == 1)
		{
			printf("\n[DEBUG]: End of file");
		}
	}


	if(verbosity == 1)
	{
	  printf("\nLength; %d", word_length[0]);
	  printf("\n");
	}
}


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
int compareWords(char *wordA, int lengthA, char *wordB, int lengthB, int verbosity)
{
    if(verbosity == 1)
    {
       printf("\n*******COMPARING********\n");
    }

    int i;
    if(lengthA != lengthB){
             if(verbosity == 1)
        {
           printf("[COMPARE] different length, length A: %d   length B: %d\n", lengthA, lengthB);
        }


        return FALSE;
    }
    else{
        for(i = 0; i<lengthA; i++){
            if(wordA[i] != wordB[i]){
                if(verbosity == 1)
                {
                    printf("[COMPARE] FALSE");
                }

                return FALSE;
            }
        }
        if(verbosity == 1)
        {
            printf("[COMPARE] TRUE\n");
        }

        return TRUE;
    }

}


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

char * copyCharArray(char *pointer, int array_length)
{
    int i;
    char *newpointer;
    newpointer = malloc(array_length*sizeof(char));
    for(i = 0; i < array_length; i++)
    {
        newpointer[i] = pointer[i];
    }
    return newpointer;

}


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

int BinCharToInt (char *pointer, int length)
{
    int i;
    char bit;
    int int_value = 0;
    for(i = 0; i < length; i++)
    {
        if (pointer[i] == '1')
        {
            int_value = int_value + pow(2,(length-i-1));
        }

    }
    return int_value;

}


/**************************************************************************/
/** @name readnextWord ****************************************************/
/**************************************************************************/
char * readnextWord()
{
	char c;
	static char word[100];
	int i = 0;


	c = fgetc(stdin);

	while((c == ' ') || (c =='\n') ){
		if(c == '\n')
		{
			word[0] = 'E';
			word[1] = 'O';
			word[2] = 'L';
  			return word;
		}
		c =fgetc(stdin);
	}


	while ((c != '\n') && (c != ' '))
	{
		printf("Character: %c\n",c);
		word[i] = c;
		c =fgetc(stdin);
		i++;
	}

	return word; //the words are equal
}


/**************************************************************************/
/** @name charToInt ****************************************************/
/**************************************************************************/
int charToInt(char c){
    int num = 0;
    num = c - '0';
    return num;
}

