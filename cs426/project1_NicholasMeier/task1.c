#include <stdio.h>
#include <stdlib.h>
void readSecretFile();

int main() {
        unsigned int x = 100;
        unsigned int y;

        unsigned int number = 42;

        printf("Input a number: ");
	/** 	Vulnerability exists here, scanf does not check if input matches format
		Likewise, instead of "%d", "%u" should be used for reading in an unsigned int
		Type = Uncontrolled Format String
	**/
        scanf("%d", &y);

        x = x + y;
	/** 	C compiler does not check if the unsigned value is truly unsigned. Thus allowing
		a variable with an unsigned descriptor to be negative if not checked. 
		Type = Missing Authentication --> (No confirmation that int is unsigned)
	**/

        if (x == 42) {
                readSecretFile();
        }
}

void readSecretFile() {

        /** Helper function, not needed for the solution */
        FILE *fptr = fopen("secretfile.txt", "r");
        if (fptr == NULL) {
                printf("cannot open, but you triggered the exploit!\n");
                exit(0);
        }
        char ch = fgetc(fptr);
        while (ch != EOF) {
                printf("%c", ch);
                ch = fgetc(fptr);
        }
        fclose(fptr);
}
