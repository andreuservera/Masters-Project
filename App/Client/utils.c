#include <math.h>
#include <stdlib.h>
#include "utils.h"


static size_t file_size(FILE *file)
{
    if (fseek(file, 0L, SEEK_END) == -1)
    {
        return FILE_ERROR;
    }

    long size = ftell(file);

    if (size == -1)
    {
        return FILE_ERROR;
    }
    if (fseek(file, 0L, SEEK_SET) == -1)
    {
        return FILE_ERROR;
    }
    return (size_t)size;
}

size_t file_get_size(const char *path)
{
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        return FILE_ERROR;
    }

    size_t size = file_size(file);

    fclose(file);
    return size;
}

static char *file_data(const char *path, const char *prefix, const char *suffix)
{
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        return NULL;
    }

    size_t size = file_size(file);

    if (size == FILE_ERROR)
    {
        return NULL;
    }

    size_t size_prefix = 0;
    size_t size_suffix = 0;

    if (prefix != NULL)
    {
        size_prefix = strlen(prefix);
    }
    if (suffix != NULL)
    {
        size_suffix = strlen(suffix);
    }

    char *str = malloc(size + size_prefix + size_suffix + 1);

    if (str == NULL)
    {
        return NULL;
    }
    if (fread(str + size_prefix, 1, size, file) != size)
    {
        free(str);
        str = NULL;
    }
    else
    {
        if (size_prefix > 0)
        {
            memcpy(str, prefix, size_prefix);
        }
        if (size_suffix > 0)
        {
            memcpy(str + size, suffix, size_suffix);
        }
        str[size + size_prefix + size_suffix] = '\0';
    }
    fclose(file);
    return str;
}

char *file_read(const char *path)
{
    return file_data(path, NULL, NULL);
}

void getWord(FILE * file_pointer,char *word, size_t *word_length, int verbosity)
{
    memset(word, 0, strlen(word));

    char c;
    //int blanks=0;
	word_length[0] = 0;
	
	if (verbosity == 1)
	{
		printf("[WORD]: ");
	}

    while((c = (char)fgetc(file_pointer)) != EOF)
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
      printf("\nLength; %zu", word_length[0]);
	  printf("\n");
	}
}


int compareWords(char *wordA, size_t lengthA, char *wordB, size_t lengthB, int verbosity)
{
    if(verbosity == 1)
    {
       printf("\n*******COMPARING********\n");
    }

    size_t i;
    if(lengthA != lengthB){
             if(verbosity == 1)
        {
           printf("[COMPARE] different length, length A: %zu   length B: %zu\n", lengthA, lengthB);
        }


        return FALSE;
    }
    else{
        for(i = 0 ; i < lengthA ; i++)
        {
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




char * copyCharArray(char *pointer, size_t array_length)
{
    size_t i;
    char *newpointer;

    newpointer = (char *) malloc((array_length + 1)*sizeof(char));
    for(i = 0; i < array_length; i++)
    {
        newpointer[i] = pointer[i];
    }
    return newpointer;

}



int BinCharToInt (char *pointer, int length)
{
    int i;
    //char bit;
    int int_value = 0;
    for(i = 0; i < length; i++)
    {
        if (pointer[i] == '1')
        {
            int_value = int_value + (int)pow(2,(length-i-1));
        }

    }
    return int_value;

}


char * readnextWord()
{
	char c;
	static char word[100];
	int i = 0;


    c = (char)fgetc(stdin);

	while((c == ' ') || (c =='\n') ){
		if(c == '\n')
		{
			word[0] = 'E';
			word[1] = 'O';
			word[2] = 'L';
  			return word;
		}
        c = (char)fgetc(stdin);
	}


	while ((c != '\n') && (c != ' '))
	{
		printf("Character: %c\n",c);
		word[i] = c;
        c = (char)fgetc(stdin);
		i++;
	}

	return word; //the words are equal
}


int charToInt(char c){
    int num = 0;
    num = c - '0';
    return num;
}

