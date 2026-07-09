#include<stdio.h>

int main(void){
	int var = 1;

	int* int_ptr = NULL;
	int_ptr = &var;

	char* char_ptr = NULL;
	char_ptr = (char*)&var;

	printf(" int : %p, char : %p \n", (void*)int_ptr, (void*)char_ptr);

//	printf(" int : %u, char : %u \n", (unsigned int)int_ptr, (unsigned int)char_ptr);

	int_ptr++; char_ptr++;
	printf(" int : %p, char : %p \n", (void*)int_ptr, (void*)char_ptr);

//	printf(" int : %llu, char : %llu \n", (unsigned long long)int_ptr, (unsigned long long)char_ptr);

	return 0;
}
