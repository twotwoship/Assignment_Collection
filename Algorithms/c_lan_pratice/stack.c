#include<stdio.h>
#define MAX 100

char pop_c(char *stack, int *top);
void push_c(char *stack, char input, int *top);

char pop_c(char *stack, int *top){
	char _pop_return = stack[*top];
	(*top)--;
	return _pop_return;
}

void push_c(char *stack, char input, int *top){
	(*top)++;
	stack[*top] = input;
}

int main(void){
	
	char stack[MAX] = { 0 };
	char * p_stack = stack;

	int top = -1;
	int *p_top = &top;
	
	char temp1 = 'z';
	char temp2 = 'c';

	push_c(p_stack, temp1,p_top);	
	push_c(p_stack, temp2,p_top);
	push_c(p_stack, temp2,p_top);

	printf("%c \n",pop_c(stack, p_top));
	printf("%c \n",pop_c(stack, p_top));
	printf("%c \n",pop_c(stack, p_top));

	return 0;
}
