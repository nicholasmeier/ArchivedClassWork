#include <stdio.h>
#include <string.h>



void function1();
void function2();



int main(int argc, char*argv[]){


	function1();

	printf("\n\nDone!!\n\n");
}



void function1(){
	printf("Enter String\n");
	char temp_string[??]; //You will need to figure out the length here
	gets(temp_string);
	printf("You Entered: %s\n", temp_string);
}

void function2(){
	//Debug - Old Testing Code. Not for Production!!!
	printf("\n\nFunction that deltes all company data from database just ran - ; ).\n\n");
}
