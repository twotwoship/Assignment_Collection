#include<stdio.h>

double div(int a, int b);
void prn(char * a);
int *save(int a);

double div(int a, int b){
	double result;
	result = (double)a / (double)b;
	return result;
}

void prn(char * a){
	
}

int *save(int a){
	
	return 0;
}

int main(void){
	
	double (*fpa)(int, int);
	void (*fpb)(char *);
	int  *(*fpc)(int);

	fpa = div;
	fpb = prn;
	fpc = save;
	
	int ary[5] = {10, 20, 30, 40, 50};
	void *vp = ary;

	printf("%d \n", *((int *)vp+2));
	return 0;
}
