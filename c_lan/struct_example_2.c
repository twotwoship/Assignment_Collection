#include<stdio.h>

struct student{
	int a;
	int b;
	int c;
};

int main(void){
	
	struct student ii;
	struct student *pi;

	ii.a = 1; ii.b = 2; ii.c = 3;

	pi = &ii;

	printf("%d %d %d \n", (*pi).a, pi->b, pi->c);
	
	struct student iii[10] = { 0 };

	for(int i = 0; i < 10; i++){
		iii[i].a = i + 1;
		iii[i].b = i + 3;
		iii[i].c = i + 5;
	}
	for(int i = 0; i < 10; i++){
		printf("%d %d %d \n",iii[i].a, iii[i].b, iii[i].c);
	}
	return 0;
}
