#include <stdlib.h>
#include <stdio.h>
char* sub_string(char*in_string, int start_index, int end_index){
	char *out_string = malloc(end_index-start_index+1);
	int x = 0;
	int y;
	for(y = start_index; y < end_index; y++){
		*(out_string+x) = *(in_string+y-1);
		x++;
	}
	*(out_string+x) = '\0';
	return out_string;
}
