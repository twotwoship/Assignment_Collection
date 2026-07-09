#include<stdio.h>
#include<stdlib.h> // atoi, itoa ..........
int print_binary(int a);

int print_binary(int a) {
	if (a == 0){
		return 0;
	}
	char temp[20][2] = { 0 };
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
		//printf("tex = %d \n", tex);

	}
	return tex;
}
int main(void) {
	char special_char[33][10] = { "NUL","SOH","STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "HT",
		"LF", "VT", "FF", "CR", "SO", "SI", "DLE",
		"DC1", "DC2", "DC3", "DC4", "NAK", "SYN",
		"ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US", "SP" };
	char list_dance[10] = { "DEL" };
	int i, i2, i3, i4;
	printf("%30s\n", "ASCII CODE TABLE");
	printf("%30s\n", "================");
	printf("%4s%4s%10s%4s%4s%4s%10s%4s%4s%4s%10s%4s%4s%4s%10s%4s\n", "DEX", "HEX", "BIN", "CHAR", "DEX", "HEX", "BIN", "CHAR", "DEX", "HEX", "BIN", "CHAR", "DEX", "HEX", "BIN", "CHAR");
	printf("%4s%4s%10s%4s%4s%4s%10s%4s%4s%4s%10s%4s%4s%4s%10s%4s\n", "===", "===", "===", "===", "===", "===", "===", "===", "===", "===", "===", "===", "===", "===", "===", "===");
	for (i = 0; i < 32; i++) {
		i2 = i + 32; i3 = i + 64; i4 = i + 96;
		if (i == 0) {
			printf("%4d%4.x%10d%4s%4d%4.x%10d%4c%4d%4.x%10d%4c%4d%4.x%10d%4s\n", i, i, print_binary(i), special_char[i], i2, i2, print_binary(i2), i2, i3, i3, print_binary(i3), i3, i4, i4, print_binary(i4), list_dance);
			//printf("%d %.x %d %s %d %.x %d %s %d %.x %d %c %d %.x %d %c\n", i, i, print_binary(i), special_char[i], i2, i2, print_binary(i2), special_char[32], i3, i3, print_binary(i3), i3, i4, i4, print_binary(i4), i4);
		}
		else if (i == 31) {
			printf("%4d%4.x%10d%4s%4d%4.x%10d%4c%4d%4.x%10d%4c%4d%4.x%10d%4s\n", i, i, print_binary(i), special_char[i], i2, i2, print_binary(i2), i2, i3, i3, print_binary(i3), i3, i4, i4, print_binary(i4), list_dance);
		}
		else {
			printf("%4d%4.x%10d%4s%4d%4.x%10d%4c%4d%4.x%10d%4c%4d%4.x%10d%4c\n", i, i, print_binary(i), special_char[i], i2, i2, print_binary(i2), i2, i3, i3, print_binary(i3), i3, i4, i4, print_binary(i4), i4);
		}
	}
	return 0;
}

