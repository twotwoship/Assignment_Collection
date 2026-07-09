#include<stdio.h>

int sum(int a, int b);
int mui(int a, int b);

int main(void){
	int a; int b; int c;
	int (*fp)(int, int);
	scanf("%d %d %d",&a, &b, &c);
	int res;
	if(a >= 0){
		fp = sum;
	}else{
		fp = mui;
	}
	res = fp(b,c);

	printf("res = %d \n",res);
	return 0;
}

int sum(int a, int b){
	int result;
	result = a + b;
	return result;
}

int mui(int a, int b){
	int result;
	result = a * b;
	return result;
}
