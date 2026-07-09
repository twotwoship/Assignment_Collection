#include<stdio.h>

struct student{
	int num;
	double grade;
};

int main(void){
	struct student ta;
	ta.num = 223413;
	ta.grade = 1134134.12413;
	
	scanf("%d",&ta.num);
	printf("%d %lf \n", ta.num, ta.grade);

	return 0;
}
