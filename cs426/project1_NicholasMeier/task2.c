#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
        char grade[5];
        char name[10];

        strcpy(grade, "nil");

        gets(name);

	/**	gets(name) is a vulnerability as it does not check the length of the string
		to the character buffer's length.
		Type = Buffer Overflow
	**/

        printf("Hi %s! Your grade is %s.\n", name, grade);

        exit(0);
}
