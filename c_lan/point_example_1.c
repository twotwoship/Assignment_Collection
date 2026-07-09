#include <stdio.h>

void print_bytes(void* data, size_t length){
	char delim = ' ';
	unsigned char* ptr = data;
	unsigned long long* ptr8 = data;
	for(size_t i = 0; i < length; i++){
		printf("%c 0x%x", delim, *ptr);
		delim = ',';
		ptr++;
	}
	printf("\n");
	printf("%c 0x%llx\n\n\n", delim,*ptr8);
}

int main(int argc, char** argv){
	int a= 23245256;
	double b = 18.9;
//	printf("%p %p %ld %ld \n",&a, &b, sizeof(int), sizeof(double));
	print_bytes(&a, sizeof(int));
	print_bytes(&b, sizeof(double));
	
	printf("%d\n", sizeof(char*));
	return 0;
}
