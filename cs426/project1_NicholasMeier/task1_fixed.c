#include <stdio.h>
#include <stdlib.h>
void readSecretFile();

int main() {
        unsigned int x = 100;
        unsigned int y;

        unsigned int number = 42;

        printf("Input a number: ");
        scanf("%u", &y);
	
	//Have y assign itself as an absolute value to ensure that it is unsigned
	y = abs(y);
        x = x + y;

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
