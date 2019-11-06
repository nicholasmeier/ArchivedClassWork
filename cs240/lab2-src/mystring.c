
#include <stdlib.h>
#include "mystring.h"

// Type "man string" to see what every function expects.

int mystrlen(char * s) {
	char *str;
	for (str = s; *str; ++str){}
	return (str - s);
	
}

char * mystrcpy(char * dest, char * src) {
	char *save = dest;
	while(*dest++ = *src++);
	return save;
}

char * mystrcat(char * dest, char * src) {
	mystrcpy(dest + mystrlen(dest), src);
	return dest;
}

int mystrcmp(char * s1, char * s2) {
	for(; *s1 == *s2; ++s1, ++s2){
		if(*s1 == 0)
			return 0;
	}
	return *(unsigned char *)s1 < *(unsigned char *)s2 ? -1: 1; 
}

char * mystrstr(char * hay, char * needle) {
	int a,b,c = 0;
	int l1 = mystrlen(hay);
	int l2 = mystrlen(needle);

	for (a = 0; a < l1; a++){
		if(hay[a] == needle[0]){
			for(b = 0; b < l2; b++){
				if (needle[b] == hay[a+b]){
					c++;
				}
				if (c == l2){
					char *z = &hay[a];
					return z;
				}
			}
		}
		c=0;
	}
	return NULL;
}

char * mystrdup(char * s) {
	char * s1;
	s1 = malloc(mystrlen(s)+1);
	mystrcpy(s1,s);
}

char * mymemcpy(char * dest, char * src, int n)
{
	int x;
	for(x = 0; x < n; x++){
		dest[x] = src[x];
	}
	return dest;
}

