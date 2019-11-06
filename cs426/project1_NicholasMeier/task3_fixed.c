#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_bad_grade(void)
{
        puts("Your grade is nil.");
        exit(0);
}

void print_good_grade(void)
{
        puts("Your grade is perfect.");
        exit(1);
}

void vulnerable()
{
        char input[4];
	/** Replace gets with fgets **/
        fgets(input, 4, stdin);
}

int main()
{
        vulnerable();
        print_bad_grade();
        return 0;
}
