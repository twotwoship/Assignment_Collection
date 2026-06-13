#include <stdio.h>
#include <string.h>
#include <stdlib.h> // atoi, itoa ..........

int int_change(char* anwer);
void binary_change(char *anwer, int num);
void print_binary(char* anwer);
int and_b(int one, int two);
int or_b(int one, int two);
int xor_b(int one, int two);
int left_shift_b(int one, int two);
int right_shift_b(int one, int two);
int reverse_b(int one);
int reverse_b_1(int one);

int and_b(int one, int two){
	int answer = 0;
	char* add_o = malloc(sizeof(char) * 33);
	char* add_t = malloc(sizeof(char) * 33);
	binary_change(add_o, one);
	binary_change(add_t, two);
	for (int i = 0; i < 32; i++) {
		if (add_o[i] == add_t[i]) {
			add_o[i] = add_o[i];
		}
		else {
			add_o[i] = 0;
		}
	}
	
	//printf("\n");
	answer = int_change(add_o);
	printf("%5d & %5d =%5d : ", one, two, answer); print_binary(add_o);

	free(add_o);
	free(add_t);
	return 0;
}

int or_b(int one, int two) {
	int answer = 0;
	char* or_o = malloc(sizeof(char) * 33);
	char* or_t = malloc(sizeof(char) * 33);
	binary_change(or_o, one);
	binary_change(or_t, two);
	for (int i = 0; i < 32; i++) {
		if ((or_o[i] == 0) && (or_t[i] == 0) ) {
			or_o[i] = 0;
		}
		else {
			or_o[i] = 1;
		}
	}
	answer = int_change(or_o);
	printf("%5d | %5d =%5d : ", one, two, answer); print_binary(or_o);

	free(or_o);
	free(or_t);
	return 0;
}

int xor_b(int one, int two) {
	int answer = 0;
	char* _o = malloc(sizeof(char) * 33);
	char* _t = malloc(sizeof(char) * 33);
	binary_change(_o, one);
	binary_change(_t, two);
	for (int i = 0; i < 32; i++) {
		if ((_o[i] == 0) && (_t[i] == 0)) {
			_o[i] = 0;
		}
		else if ((_o[i] == 1) && (_t[i] == 1)) {
			_o[i] = 0;
		}
		else {
			_o[i] = 1;
		}
	}
	answer = int_change(_o);
	printf("%5d ^ %5d =%5d : ", one, two, answer); print_binary(_o);

	free(_o);
	free(_t);
	return 0;
}

int left_shift_b(int one, int two) {
	int answer = 0;
	char* _o = malloc(sizeof(char) * 33);
	answer = one >> two;
	binary_change(_o, answer);
	printf("%5d >>%5d =%5d : ", one, two, answer); print_binary(_o);
	return 0;
}

int right_shift_b(int one, int two) {
	int answer = 0;
	char* _o = malloc(sizeof(char) * 33);
	answer = one << two;
	binary_change(_o, answer);
	printf("%5d <<%5d =%5d : ", one, two, answer); print_binary(_o);
	return 0;
}

int reverse_b(int one) {
	int answer = one;
	char* _o = malloc(sizeof(char) * 33);
	binary_change(_o, answer);
	for (int i = 0; i < 32; i++) {
		if (_o[i] == 0) {
			_o[i] = 1;
		}
		else {
			_o[i] = 0;
		}
	}
	answer = int_change(_o);
	printf("       ~%5d =%5d : ", one, answer); print_binary(_o);
	return 0;
}

int reverse_b_1(int one) {
	int answer = one;
	char* _o = malloc(sizeof(char) * 33);
	binary_change(_o, answer);
	for (int i = 0; i < 32; i++) {
		if (_o[i] == 0) {
			_o[i] = 1;
		}
		else {
			_o[i] = 0;
		}
	}
	answer = int_change(_o);
	answer++;
	printf(" ~%5d +   1 =%5d : ", one, answer); print_binary(_o);
	return 0;
}


int int_change(char* anwer) { // 음수 생각해야됨 2의 보수
	int answer = 0;
	if (anwer[0] == 0) { // 양수일때 첫짜리로 체크하는거임.
		for (int i = 1; i < 32; i++) { // anwer[0]은 무시하고 1부터 체크해서 31까지 가야됨.
			// anwer[1] i가 1이면 2의 30의자리 i가 31이면 i는 1이여야됨 2의 0승
			//answer += (2 * (32 - i)) * anwer[i]; // 아 곱연산이 안되고잇구나 곱연산 구현 어케함? for문 밖에 없나?
			if (anwer[i] != 0) {
				//printf("i = %d", i );
				int temp = 1;
				for (int j = 1; j <= 31 - i; j++) {
					temp = temp * 2;
				}
				answer += temp;
			}
			//printf("anwer[i] = %d, pharse = %d, answer = %d\n", anwer[i], 32-i, answer);
		}
	}
	else { // 음수일때
		for (int i = 1; i < 32; i++) { // anwer[0]은 무시하고 1부터 체크해서 31까지 가야됨.
			// anwer[1] i가 1이면 2의 30의자리 i가 31이면 i는 1이여야됨 2의 0승
			//answer += (2 * (32 - i)) * anwer[i]; // 아 곱연산이 안되고잇구나 곱연산 구현 어케함? for문 밖에 없나?
			if (anwer[i] == 0) {
				//printf("i = %d", i);
				int temp = 1;
				for (int j = 1; j <= 31 - i; j++) {
					temp = temp * 2;
				}
				answer -= temp;
			}
			//printf("anwer[i] = %d, pharse = %d, answer = %d\n", anwer[i], 32 - i, answer);
		}
		answer--;
	}
	return answer;
}
void binary_change(char *anwer, int num) {
	//printf("binary_in = %d\n", num);
	char temp[33] = "0";
	for (int i = 0; i < 32; i++) {
		anwer[i] = 0;
		temp[i] = 0;
	}

	// 10진법 2진법으로 변환.
	int count = 0;
	for (count; num >= 1; count++) {
		int result = num % 2;
		num /= 2;
		temp[count] = result;
		//printf("num = %d, binary_in = %d, temp[count] = %d count = %d\n", num, result, temp[count], count);
	}
	/*
	for (int i = 0; i < 32; i++) {
		printf("%d ", temp[i]);
	}
	printf("\n");
	*/
	//거로 집어 넣어야됨. 어케? 0011 거꾸로 채워 넣으면 되지
	
	for (int i = 31-count; count >= 0; i++, count--) {
		anwer[i] = temp[count];
	}
	//print_binary(anwer);
}

void print_binary(char* anwer) {
	for (int i = 0; i < 32; i++) {
		printf("%d", anwer[i]);
		if (i % 4 == 3) {
			printf(" ");
		}
	}
}

int main(void) {
	char buff[40];
	char temp[40];
	int one;
	int two;
	int check = 0;
	
	char *anwer = malloc(sizeof(char) * 33);
	char* *two_b = malloc(sizeof(char) * 33);

	printf("비트연산을 수행할 두 개의 정수를 입력하세요. >> ");
	fgets(buff, 40, stdin);
	for (check; check < 40; check++) {
		if (buff[check] != 32) {
			temp[check] = buff[check];
		}
		else {
			one = atoi(temp);
			for (int j = 0; j < 40; j++) {
				temp[j] = '\0';
			}
			break;
		}
	}
	for (int i = 0; check < 40; check++, i++) {
		if (buff[check] != '\0') {
			temp[i] = buff[check];
		}
		else {
			two = atoi(temp);
			break;
		}
	}
	binary_change(anwer, one);
	binary_change(two_b, two);
	printf("\n");
	printf("%20d : ", one); print_binary(anwer); printf("\n");
	printf("%20d : ", two); print_binary(two_b); printf("\n\n");
	and_b(one, two); printf("\n\n");
	or_b(one, two); printf("\n\n");
	xor_b(one, two); printf("\n\n");
	left_shift_b(one, two); printf("\n\n");
	right_shift_b(one, two); printf("\n\n");
	reverse_b(one); printf("\n\n");
	reverse_b(two); printf("\n\n");
	reverse_b_1(one); printf("\n\n");
	reverse_b_1(two); printf("\n\n");

	return 0;
}
