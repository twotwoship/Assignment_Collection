#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100
#define QMAX 10

int qfront = 0;
int qrear = 0;
int qcount = 0;

void enqueue(char queue[][MAX]);
char *dequeue(char queue[][MAX]);

void enqueue(char queue[][MAX]){
	
	scanf("%99s", queue[qrear]);
	qrear = (qrear + 1) % QMAX;
	qcount++;
}

char *dequeue(char queue[][MAX]){
	
	char *inbuff = queue[qfront];
	qfront = (qfront + 1) % QMAX;
	qcount--;
	return inbuff;
}


int main(void){
	char *inbuff;
	char (*queue)[MAX] = (char (*)[MAX])malloc(sizeof(*queue) * QMAX);
	char *outbuff = (char *)malloc(sizeof(char) * MAX);
	char *sstack = (char *)malloc(sizeof(char) * MAX);
	int *istack = (int *)malloc(sizeof(int) * MAX);


	enqueue(queue);
	inbuff = dequeue(queue);
	
	if(strcmp(inbuff, "exit") == 0){
		printf("프로그램이 종료 되었습니다 !!!!!\n");
	}
	return 0;
}
