
#include <stdio.h>
#include "stack.h"
#include <stdlib.h>

int top=0;
double stack[MAXSTACK];

void stack_clear() 
{
  top = 0;
}

double stack_pop()
{
	double temp = stack[--top];
	return temp;
}

void stack_push(double val)
{
	stack[top] = val;
	top++;
}

void stack_print()
{
	printf("Stack:\n");
	int x;
	if (top != 0){
		for (x = 0; x < top; x++){
			printf("%d: ", x);
			printf("%f\n", stack[x]);
		}
	}else{
		printf("Stack is empty");
	}
}

int stack_top()
{
  return top;
}

int stack_max()
{
  return MAXSTACK;
}

int stack_is_empty()
{
  return top == 0;
}


