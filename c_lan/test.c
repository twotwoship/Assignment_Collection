#include <stdio.h>

void _add(int *a, int *b, int *answer);
void _mul(int *a, int *b, int *answer);
void print_ary(int *pa);
void (*pfunc[])(int *a, int *b, int *c) = { _add, _mul };

int ary[5] = {1,2,3,4,5};
int main(void){
	
	int a; int b;
	int answer;

	int *pa = &a;
	int *pb = &b;
	int *panswer = &answer;

	a = 5; b = 10;
	
	answer = 0;

	
	pfunc[1](pa, pb, panswer);

	printf("answer : %d\n",answer);

	print_ary(ary);
	return 0;
}

void print_ary(int pa[5]){
	int i ;
	for(i = 0; i < 5; i++){
		printf("%d \n",pa[i]);
	}
}

void _add(int *a, int *b, int *answer){
	*answer = *a + *b;
}

void _mul(int *a, int *b, int *answer){
	*answer = (*a)*(*b);
}
