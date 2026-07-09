#include <stdio.h>

char* binary_string(int n)
{
	static char bin[33];
    int started = 0;
    int idx = 0;

    for (int i = 31; i >= 0; i--) {
        int bit = (n >> i) & 1;

        if (bit == 1)
            started = 1;

        if (started)
            bin[idx++] = bit + '0';
    }

    if (idx == 0)
        bin[idx++] = '0';

    bin[idx] = '\0';
	
	return bin;
}

int main(void){

    binary_string(452);
	int i = 0;
	printf("%4d %4X %10s\n", i, i, binary_string(1233));

    return 0;
}
