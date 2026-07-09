#include<stdio.h>
#include<stdlib.h>
int main(void){
	int *pi;
	double *pd;

	pi = (int *)malloc(sizeof(int));
	if(pi == NULL){
		printf("not enough energy\n");
		exit(1);
	}
	pd = (double *)malloc(sizeof(double));
	if(pd == NULL){
		printf("not enough energy\n");
		exit(1);
	}
	*pi = 10; *pd = 4.24532;

	printf("intergraaaa : %d, flooooot : %lf \n", *pi, *pd);

	free(pi);
	free(pd);
	return 0;
}
