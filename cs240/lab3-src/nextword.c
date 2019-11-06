
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Separates the file into words
//

#define MAXWORD 200
char word[MAXWORD];
int wordLength;

// It returns the next word from fd.
// If there are no more more words it returns NULL. 
char * nextword(FILE * fd) {
	int c;
	int x = 0;
	memset(word, 0, strlen(word));
	while((c=fgetc(fd))!=EOF){
		if( (c != ' ') && (c != '\n') && (c != '\t') && (c != '\r') ){
			word[x] = c;
			x++;
		}else{
			if (x > 0){
				return word;
				x = 0;
			}
		}
	}
	// While it is not EOF read char
		// While it is not EOF and it is a non-space char
		// store character in word.
		// if char is not in word return word so far.
		//
	// Return null if there are no more words
	return NULL;
}

