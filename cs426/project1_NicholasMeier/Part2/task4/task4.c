#include <stdio.h>
#include <string.h>

void authenticate();

int main(int argc, char* argv[]){

	authenticate();

	return 0;
}


void authenticate(){

	unsigned char pass = 0;
	char buff[XX]; //YOu will need to figure out the buffer length

	printf("\nEnter your password : \n");
	gets(buff);

	if(strcmp(buff, "??????")){ //You will need to figure out the correct password
		printf("\nWrong Password \n");
	}
	else {
		printf("\nCorrect Password \n");
		pass = 1;
	}

	if (pass){
		/* Give elevated privileges to user */
		printf("\nElevated privileges given to the user ...\n");
		while(1){
			//Do Privileged Actions
		}
	} else {
		printf("Exiting\n");
	}
}
