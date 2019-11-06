#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Problem (1/4)
/******************************************************************************
 * TODO: Print the bits in bitmap as 0s and 1s
 * 
 * Parameters: bitmap -- print the binary representation of this number
 *
 * Return: void 
 *
 * Return Type: void
 *****************************************************************************/
void printBits(unsigned int bitmap)
{
	char z[] = "10987654321098765432109876543210";
	unsigned int s = sizeof(unsigned int);
	unsigned int mp = 1<<(s*8-1);
	int i;
	unsigned int ar[(s*8-1)];
	for (i = 0; i < s*8; i++){
		ar[i] = (bitmap&mp ? 1 : 0);
		bitmap = bitmap<<1;
	}
	for (i = 0; i < s*8; i++){
		printf("%u", ar[i]);
	}
	printf("\n");
	printf("%s\n", z);
}


// Problem (2/4)
/******************************************************************************
 * TODO: Set the ith bit in *bitmap with the value in bitValue (0 or 1)
 * 
 * Parameters: bitmap -- unsigned integer
 *	       i -- index of the bit to be changed
 *             bitValue -- change the ith bit to this value
 *
 * Return: void 
 *
 * Return Type: void
 *****************************************************************************/
void setBitAt( unsigned int *bitmap, int i, int bitValue ) 
{
	*bitmap ^= (1 << i);	
}

// Problem (3/4)
/******************************************************************************
 * TODO: Return the bit value (0 or 1) at the ith bit of the bitmap
 * 
 * Parameters: bitmap -- unsigned integer
 *	       i -- index of the bit to be retrieved
 *
 * Return: the ith bit value 
 *
 * Return Type: integer
 *****************************************************************************/
int getBitAt( unsigned int bitmap, unsigned int i) {
	unsigned int s = sizeof(unsigned int);
	unsigned int mp = 1<<(s*8-1);
	int x; 
	unsigned int ar[(s*8-1)];
	for (x = 0; x < s*8; x++){
		ar[x] = (bitmap&mp ? 1 : 0);
		bitmap = bitmap<<1;
	}
	return ar[31-i];
}

// Problem (4/4)
/******************************************************************************
 * TODO: Return the number of bits with the value 'bitValue'
 *	 If the bitValue is 0, return the number of 0s. If the bitValue is 1,
 *	 return the number of 1s.
 * 
 * Parameters: bitmap -- unsigned integer
 *	       bitValue -- count how many times this number, either 0 or 1,
 *	       appears in bitmap
 *
 * Return: count of 0s or 1s in bitmap 
 *
 * Return Type: integer
 *****************************************************************************/
int countBits( unsigned int bitmap, unsigned int bitValue) {
	int count = 0;
	unsigned int s = sizeof(unsigned int);
	unsigned int mp = 1<<(s*8-1);
	unsigned int ar[(s*8-1)];
	int x;
	for (x = 0; x < s*8; x++){
		ar[x] = (bitmap&mp ? 1 : 0);
		bitmap = bitmap<<1;
	}
	for (x = 0; x < s*8; x++){
		if (ar[x] == bitValue){
			count++;
		}
	}
	return count;
}

