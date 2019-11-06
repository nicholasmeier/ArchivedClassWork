#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_DIGITS 32

char ba[MAX_DIGITS];
char aa[MAX_DIGITS];

// Problem (1/2)
/*****************************************************************************
 * TODO: convert a number from the given base to decimal
 *
 * Parameters: number -- the number you are converting an integer
 *             base   -- the base of the number you are given
 * 
 * Return: The number as an integer
 *
 * Return Type: int
*****************************************************************************/
int toInteger(char * number, int base) {
	int iger = strtol(number, NULL, base);
	return iger;
}

// Problem (2/2)
/*****************************************************************************
 * TODO: convert a number from the given base to decimal
 *
 * Parameters: number -- the number you are converting a string
 *             base   -- the base you are converting the numebr to
 * 
 * Return: The number as a string in base "base"
 *
 * Return Type: char *
*****************************************************************************/
char * toBase(int number, int base) {
	memset(ba, 0, strlen(ba));
	memset(aa, 0, strlen(aa));
	if (base == 10){
		sprintf(ba, "%d", number);
		return ba;
	}else{
		int temp = number;
		int x = 0;
		int a;
		char b;
		while (temp > 0){
			a = temp%base;
			temp = temp/base;
			if ((base > 10) && (a > 9)){
				switch(a){
					case 10: 
						b = 'A';
						break;
					case 11:
						b = 'B';
						break;
					case 12:
						b = 'C';
						break;
					case 13:
						b = 'D';
						break;
					case 14:
						b = 'E';
						break;
					case 15:
						b = 'F';
						break;
					case 16:
						b = 'G';
						break;
					default:
						printf("FUCK\n");
					}		
			}else{
				b = a + '0';
			}
			ba[x] = b;
			x++;
			
		}
		int balen = 0;
		while (ba[balen]!='\0'){
			balen++;
		}
		int v;
		int u = 0;
		for (v = (balen-1); v >= 0; v--){
			aa[u] = ba[v];
			u++;
		}
		aa[u] = '\0';
	}
	return aa;
}
