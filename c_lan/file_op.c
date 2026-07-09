#include<stdio.h>

int main(void){
	FILE *fp;
	int ch;
	char str[] = "tttttttesssssssssssssttttttttt";
	int i;
	/* **************************__file_read_type
	fp = fopen("test.txt", "r");
	if( fp == NULL){
		printf("gg\n");
		return 1;
	}
	printf("open_file\n");

	while(1){
	ch = fgetc(fp);
		if(ch == EOF){
			break;
		}
		putchar(ch);
	}	
	fclose(fp);

	*/
	fp = fopen("test.txt","w");
	if( fp == NULL){
		printf("gg\n");
		return 1;
	}
	i = 0;
	while(str[i] != '\0'){
		fputc(str[i], fp);
		i++;
	}
	fputc('\n', fp);
	fclose(fp);
	return 0;
}
