#include<stdio.h>

char* binary_1(int a){
	static char bin[33];
	int start = 0;
	int idx = 0;
	for(int i = 31; i >= 0; i--){
		// 오른쪽으로 비트정렬
		int bit = (a >> i) & 1;
		
		if(bit == 1){
			start = 1;
		}
		if(start){
			bin[idx++] = bit + '0';
		}
	}

	if( idx == 0){
		bin[idx++] = '0';
	}
	bin[idx] = '\0';

	return bin;
}

int main(void){

	printf("%s\n", binary_1(12431));
	return 0;
}
