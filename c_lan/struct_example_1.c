#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct student{
	char name[20];
	int age;
	double height;
	char *intro;
};

int main(void){
	char nan[20];
	struct student yang;
	fgets(yang.name, sizeof(yang.name), stdin);
	fgets(nan, sizeof(nan),	stdin);
	yang.age = atoi(nan);
	fgets(nan, sizeof(nan), stdin);
	yang.intro = (char *)malloc(sizeof(nan));
	strcpy(yang.intro, nan);

	printf("name : %s",yang.name);
	printf("age : %d\n", yang.age);
	printf("intro : %s", yang.intro);
	return 0;
}
