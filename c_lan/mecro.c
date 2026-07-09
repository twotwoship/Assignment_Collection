#include<stdio.h>

void func(void);

int main(void){
	printf("compile date and time : %s, %s\n\n", __DATE__, __TIME__);
	printf("files : %s\n", __FILE__);
	printf("funs : %s\n", __FUNCTION__);
	printf("low number : %d\n", __LINE__);

#line 100 "macro.c"
	func();
	return 0;
}

void func(void){
	printf("\n");
	printf("files : %s\n", __FILE__);
	printf("funs : %s\n", __FUNCTION__);
	printf("low number : %d\n", __LINE__);

}
