int print_binary(int a);

int print_binary(int a) {
	char temp[10] = { 0 };
	int check = 0;
	int temp_num = 0;
	int answer = 0;
	int tex = 0;
	while (a <= 1) {
		temp_num = a % 2;
		temp[check] = temp_num;
		check++;
	}
	check++;

	temp[check] = temp_num; //여기서 바이너리로 만들었음.  24 = 10100 00101로 들어감 아니지 그냥 숫자로 만들어버리면됨.
	for (int i = 0; i < check; i++) {
		//int ttemp = atoi(temp[i]); //이렇게 쓰면 안되는듯 하나하나 뽑아서 쓸수없다. 그러면 만들면되지 그렇게
		int ttemp = (int)temp[i];
		tex += (ttemp * (10*(i)));
	}

	return tex;
}
#include<stdio.h>
#include<stdlib.h> // atoi, itoa ..........

int main(void) {
	char special_char[33][10] = { "NUL","SOH","STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "HT",
		"LF", "VT", "FF", "CR", "SO", "SI", "DLE",
		"DC1", "DC2", "DC3", "DC4", "NAK", "SYN",
		"ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US", "SP" };
	char list_dance[10] = { "DLE" };
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
