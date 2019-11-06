
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

#include "rpn.h"
#include "nextword.h"
#include "stack.h"


double rpn_eval(char * fileName, double x) {
	
	double a, b, c, z;
	FILE *fd = fopen(fileName, "r");

	char * word;
	stack_clear();
	while ((word=nextword(fd))!=NULL) {
	//	stack_print();
	//	printf("word %s\n",word);
		if ( (word[0] == '1') || (word[0] == '2') || (word[0] == '3') || (word[0] == '4') || (word[0] == '5') || (word[0] == '6') || (word[0] == '7') || (word[0] == '8') || (word[0] == '9') || (word[1] == '0') || (word[1] == '1') || (word[1] == '2') || (word[1] == '3') || (word[1] == '4') || (word[1] == '5') || (word[1] == '6') || (word[1] == '7') || (word[1] == '8') || (word[1] == '9') ){
			sscanf(word, "%lf", &z);
	//		printf("word %s\n",word);
	//		printf("z %f\n", z);
			stack_push(z);		
		}
		
		if (word[0] == 'x'){
			stack_push(x);
		}

		if ((strcmp(word,"+")) == 0){
			if (stack_top() == 1){
				printf("Stack underflow\n");
				exit(0);
			}
			a = stack_pop();
	//		printf("a %f\n",a);
			b = stack_pop();
	//		printf("b %f\n",b);
			c = a + b;
			stack_push(c);
	//		printf("c %f\n",c);
		}
			
		if ((strcmp(word, "-")) == 0){
			if (stack_top() == 1){
				printf("Stack underflow\n");
				exit(0);
			}

			a = stack_pop();
			b = stack_pop();
			c = b - a;
			stack_push(c);
		}
		if ((strcmp(word, "*")) == 0){
			if (stack_top() == 1){
				printf("Stack underflow\n");
				exit(0);
			}

			a = stack_pop();
			b = stack_pop();
			c = a * b;
			stack_push(c);
		}
		if ((strcmp(word, "/")) == 0){
			if (stack_top() == 1){
				printf("Stack underflow\n");
				exit(0);
			}

			a = stack_pop();
			b = stack_pop();
			c = b/a;
		 	stack_push(c);
		}
		if ((strcmp(word, "sin")) == 0){
			a = stack_pop();
			c = sin(a);
			stack_push(c);
		}
		if ((strcmp(word, "cos")) == 0){
			a = stack_pop();
			c = cos(a);
			stack_push(c);
		}
		if ((strcmp(word, "pow")) == 0){
			if (stack_top() == 1){
				printf("Stack underflow\n");
				exit(0);
			}
			a = stack_pop();
			b = stack_pop();
			c = pow(b,a);
			stack_push(c);
		}
		if ((strcmp(word, "log")) == 0){
			a = stack_pop();
			c = log(a);
			stack_push(c);
		}
		if ((strcmp(word, "exp")) == 0){
			a = stack_pop();
			c = exp(a);
			stack_push(c);
		}

	}
	if (stack_top() > 1){
		printf("Elements remain in the stack\n");
		exit(0);
	}
	return stack_pop(); 
}

