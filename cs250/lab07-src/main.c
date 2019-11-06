#include <stdio.h>

main(){
	char str1[100];
	printf("Enter string: ");
	scanf("%s", str1);
	int start;
	int end;
	printf("Enter start index: ");
	scanf("%d", &start);
	printf("Enter end index: ");
	scanf("%d", &end);
	//printf("%d\n", &end);
	//printf("%d\n", &start);
	//printf("%s\n", str1);
	sub_string(str1, start, end); //call sub_string func with args
	/*
	char str2[(end-start)+1];
	int x = 0;
	int y = 0;
	for (x = 0; x < (end-start); x++){
		str2[x] = str1[start+x];
	}
	str2[(end-start)] = '\0';
	printf("%s\n", str2);
	*/
}
