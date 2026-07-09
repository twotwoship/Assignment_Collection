#include<stdio.h>

int binary_print(int a){
	int result = 0;
	for(int i = 7; i >= 0; i--){
		result *= 10;
		result += (a >> i) & 1;
	}
	return result;
}

int main(void){
	int a = 32;
	a = binary_print(20);
	printf("%d\n", a);
	return 0;
}
