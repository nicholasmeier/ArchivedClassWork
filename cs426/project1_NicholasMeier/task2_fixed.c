#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
        char grade[5];
        char name[10];

        strcpy(grade, "nil");
	
        /** Change gets(...) to fgets(...) **/
	fgets(name, 10, stdin);

        printf("Hi %s! Your grade is %s.\n", name, grade);

        exit(0);
}
