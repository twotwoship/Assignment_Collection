#include<stdio.h>
#define max 100

int main(void){

	int list[max+1] = { 0 };
	
	list[0] = 1;
	list[1] = 1;
	for(int i = 2; i * i <= max+1; i++){
		if(list[i] == 0){
			for(int j = i*i; j <= max; j += i){
				list[j] = 1;
			}
		}
	}

	for(int i = 0; i <= max; i++){
		if(list[i] == 0){
			printf("sosu = %d\n",i);
		}
	}
	return 0;
}
