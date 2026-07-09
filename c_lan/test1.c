#include<stdio.h>
#include<stdlib.h> // atoi, itoa ..........
int print_binary(int a);

int print_binary(int a) {
	char temp[10][2] = { 0 };
	int check = 0;
	int temp_num = 0;
	int answer = 0;
	int tex = 0;
	while (1) {
		if(a == 1){
			break;
		}
		temp_num = a % 2;
		a = a / 2;
		temp[check][0] = temp_num;
		check++;
	}
	temp[check][0]= 1; //여기서 바이너리로 만들었음.  24 = 10100 00101로 들어감 아니지 그냥 숫자로 만들어버리면됨.
	check++;
	for (int i = 0; i < check; i++) {
		int ttemp = temp[i][0];
		int base =10;
		int ans = 1;
		for(int j = 0; j < i;  j++){
			 ans *= base;
		}
		tex += ttemp * ans;
		printf("tex = %d \n", tex);

	}

	return tex;
}
int main(void){
	printf("%d", print_binary(24));
	return 0;
}
