#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void print_str(char **ps);

int main(void){
	char temp[80];
	char *str[21] = { 0 };
	int check = 0;	
	while(check < 20){
		printf("input : ");
		fgets(temp, sizeof(temp), stdin);
		if(strcmp(temp, "end\n") == 0) break;
		str[check] = (char *)malloc(strlen(temp);
		strcpy(str[check], temp);
		check++;
	}
	print_str(str);

	for(int i = 0; str[i] != NULL; i++){
		free(str[i]);
	}
	return 0;
}

void print_str(char **ps){
	while( *ps != NULL){
		printf(" %s", *ps);
		ps++;
	}
}
