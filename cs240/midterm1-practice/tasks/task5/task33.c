/*
 *  CS 24000 First mid-semester exam.
 *   Problem #3-3
 *
 *  void censorWord(char * str, char * badword)
 *
 *  Arguments:
 *		char * str -- a pointer to a string
 *      char * badword -- a word to replace in string
 *		
 *  Description:
 *		Given a string of input, replace all occurrences of the word badword in str 
 *		with X's. The word must have whitespace on both sides (space, tab, newline
 *		or carriage return), unless it is at the beginning/end of the string str, 
 *		and must match case. Word will not have any whitespace characters in it.
 *		For example, 
 *			str = "The penguin is in the theater"
 *			censorWord(str, "the");
 *			This would result in str pointing to "The penguin is in XXX theater"
 *
 */

 /* ======================== Start your edits here =========================== */
 
 /*
  *  NOTE NOTE NOTE NOTE NOTE
  *  You are not allowed to add any other libraries or library includes in addition
  *   to <stdlib.h> other than <math.h> (if you believe you need it).
  */
 #include <stdlib.h>
 /* 
  * Complete the function below for the exam
  */
void censorWord(char * str, char * badword)
{
	int lenbad = 0;
	int lenstr = 0;
	while(str[lenstr]!='\0'){
		lenstr++;
	}
	while(badword[lenbad]!='\0'){
		lenbad++;
	}
	

	int x;
	int y;
	int cnt = 0;
	for (x = 0; x < lenstr; x++){
		if (str[x] == badword[0]){
			for(y = 0; y < lenbad; y++){
				if (str[(x+y)] == badword[y]){
					cnt++;
				}
			}
			if ((cnt == lenbad)&&( (str[x+lenbad] == ' ') || (str[x+lenbad]=='\0') ) ){
				for(y = 0; y < lenbad; y++){
					str[x+y] = 'X';
				}
			}
		}
		cnt = 0;
	}
	
}
  
 /* ======================== End your edits here =========================== */

 /*
 ************************************************************************
  *  Do NOT change any of the lines between here and the end of the file!
 ************************************************************************
  */

#include <stdio.h>
#include <assert.h>

char *Usage = "Usage: %s <string> <word>\n";

int main(int argc, char *argv[])
{
	extern char * strdup(const char *);
	char * copy;
	
	if (argc != 3) 
	{
		fprintf(stderr, Usage, argv[0]);
		exit(1);
	}
	assert(copy = strdup(argv[1]));
	censorWord(copy, argv[2]);

	printf("String \"%s\" after censorWord is \"%s\"\n", argv[1], copy);
	
	return 0;
}
	
	
