int myprintf(char * format, ...);
#include <stdio.h>
int main(){
	myprintf("%d %c %s %x %s\n", 32767, '\t', "tab padding", -1168197103, " game");
	return 0;
}

