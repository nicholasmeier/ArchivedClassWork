#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Problem (1/4)
/******************************************************************************
 * TODO: Print the array.
 *       The format should be (array index)(colon)(array element)(newline) 
 * 
 * Parameters: n -- the number of elements in the array
 *             array -- a double array
 *
 * Return: void 
 *
 * Return Type: void
 *****************************************************************************/
void printArray(int n, double * array) {
	int x;
	for (x = 0; x < n; x++){
		double z = array[x];
		printf("%d:%f\n", x, z);
	}
}

// Problem (2/4)
/******************************************************************************
 * TODO: Return the minimum element of array 
 * 
 * Parameters: array -- a double array
 *             n -- the number of elements in the array
 *
 * Return: minimum element in array 
 *
 * Return Type: double
 *****************************************************************************/
double minArray(int n, double * array) {
// Write Your Code Here
	double * z = array;
	int x;
	double min;
	for (x = 0; x < n; x++){
		double a = *(z+x);
		if (x == 0){
			if (a < *(z+x+1)){
				min = a;	
			}
		}
		if (a < min){
			min = a;
		}
	}
	return min;
}

// Problem ( 3/4 ) 
/******************************************************************************
 * TODO: Reverse the given string 'str'. 
 * E.g. reverse_str("smile") should return "elims"
 * 
 * Parameters: str -- The given string to be reversed.
 *
 * Return: A pointer to str, str should be reversed 
 *
 * Return Type: char pointer
 *****************************************************************************/
char * reverse_str (char * str ) {
	char * rev;
	rev = malloc(strlen(str)+1);
	int x;
	int y = (strlen(str)-1);
	for (x = 0; x < strlen(str); x++){
		*(rev+x) = *(str+y);
		y--;
	}
	for(x = 0; x < strlen(str); x++){
		*(str+x) = *(rev+x);
	}
	return str;

}

// Problem ( 4/4 ) 
/******************************************************************************
 * TODO: Determine if the string str2 is a permutation of str1. A permutation
 * is the rearranging of characters in a different order. 
 * E.g. the string "act" is a permutation of "cat" 
 *
 * Hint: count the occurences of each letter
 * 
 * Parameters: str1 -- The original string
 *	       str2 -- Determine if this string is a permutation of str1 
 *
 * Return: 1 if str2 is a permutation
 *         0 if str2 is not a permutation
 *
 * Return Type: integer
 *****************************************************************************/
int is_permutation ( char * str1, char * str2 ) {
// Write Your Code Here    
	char * s1 = str1;
	char * s2 = str2;
	int x = 0;
	int y = 0;
	int z = 0;
	int w = -1;
	if (strlen(str1) != strlen(str2)){
		return 0;
	}
	for (x = 0; x < (strlen(str1)); x++){
		for (y = 0; y < (strlen(str1)); y++){
			if ((y == w)){
				if (y == (strlen(str1)-1)){
					break;
				}
				y++;
			}
	
			if (*(str1+x) == *(str2+y)){
				w = y;
				z++;
				break;
			}
		}
	}
	if (z == strlen(str1)){
		return 1;
	}
	return 0;
}
