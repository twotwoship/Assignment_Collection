#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(void){
	
	char temp[80];
	char *p_temp;
	int temp_l;
	fgets(temp, 80, stdin);
	p_temp = (char *)malloc(strlen(temp) + 1);
	strcpy(p_temp, temp);

	printf("%s",p_temp);
	return 0;
}
