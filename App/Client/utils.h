#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILE_ERROR ((size_t)-1)
#define TRUE 1
#define FALSE 0

size_t file_get_size(const char *);
char *file_read(const char *);
void getWord(FILE * file_pointer, char *word, size_t *word_length, int verbosity);
int compareWords(char *wordA, size_t lengthA, char *wordB, size_t lengthB, int verbosity);
char * copyCharArray(char *pointer, size_t array_length);
int BinCharToInt (char *pointer, int length);
char * readnextWord(void);
int charToInt(char c);


#endif // UTILS_H
