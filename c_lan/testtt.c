#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*
what is variable? plate is putting data 데이터를 담는 그릇이다.
what is pointer? plate is putting address 주소를 담는 그릇이다.
why put address? number_one : other variable of function access(read, write) availble
주소를 담는 이유? 서로다른 함수에 있는 변수에 접근하기 위함.
*/

void shift_alpha(char* pa);
void shift_alpha(char* pa) {
	char temp;
	for (int i = 0; i < 25; i++) {
		printf("%s\n", pa);
		temp = pa[0];
		for (int j = 0; j < 24; j++) {
			pa[j] = pa[j + 1];
		}
		pa[24] = temp;

	}
}

int main(void){
	char a[] = "ABCEFGHIJKLMNOPQRSTUVWXYZ";
	shift_alpha(a);
	return 0;
}
