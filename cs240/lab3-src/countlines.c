
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char ** argv) {
	printf("Program that counts lines.\n");
	int c;
	int lines=0;

 	if (argc < 2){
		printf("Ussage: countlines <filename>\n");
		exit(1);
	}
	
	char * fileName = argv[1];
	FILE *fd = fopen(fileName, "r");
	if (fd == NULL){
		printf("Could not open file %s\n", fileName);
		exit(1);
	}
	while((c=fgetc(fd))!=-1){
		if (c == '\n'){
			lines++;
		}
	}
	printf("Total lines: %d", lines);
	printf("\n");
	exit(0);
	
}
