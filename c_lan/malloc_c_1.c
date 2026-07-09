#include<stdio.h>
#include<stdlib.h>

int main(void){
	int *pi;
	int i, sum = 0;

	pi = (int *)malloc(5 * sizeof(int));
	if(pi == NULL){
		exit(1);
	}
	printf("asdf : ");
	for(int i = 0; i < 5; i++){
		scanf("%d", &pi[i]);
		//sum += pi[i];
		sum += *(pi + i);
	}
	printf("sum = %d",sum);
	free(pi);
	return 0;
}
