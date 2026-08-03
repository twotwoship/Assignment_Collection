// 2026.08.01 시작시각 09:51 종료시각 11:01

#include<stdio.h>
#include<stdlib.h>
#

#define MAX 100
#define QUEUE_MAX 10

char error_code = '0';

int q_front = 0;
int q_rear = 0;
int q_count = 0;

void back_change(char *inbuff, char *outbuff, char *stack, int *top, int *ccr, int *ocr);
int back_calcul(char *outbuff, int *stack, int *top);

void enqueue(char queue[][MAX]);
char *dequeue(char queue[][MAX]);

char cpop(char *stack, int *top);
void cpush(char *stack, int *top, char a);
char cscan(char *stack, int *top);

int ipop(int *stack, int *top);
void ipush(int *stack, int *top, int a);
int iscan(int *stack, int *top);

void enqueue(char queue[][MAX]){
	if(q_count >= QUEUE_MAX){
		error_code = 'g';
		return;
	}

	scanf(" %99[^\n]", queue[q_rear]);
	q_rear = (q_rear + 1) % QUEUE_MAX;
	q_count++;
}

char *dequeue(char queue[][MAX]){
	if(q_count <= 0){
		error_code = 'h';
		return NULL;
	}

	char *inbuff = queue[q_front];
	q_front = (q_front + 1) % QUEUE_MAX;
	q_count--;
	return inbuff;
}

void back_change(char *inbuff, char *outbuff, char *stack, int *top, int *ccr, int *ocr){
	*top = -1;
	int j = 0;
	for(int i = 0; i < MAX; i++){
		if(inbuff[i] == '=' || inbuff[i] == '\n' || inbuff[i] == '\0'){
			while(!(*top == -1)){
				outbuff[j++] = cpop(stack, top);
			}
			outbuff[j] = '=';
			if(*ocr != *ccr){
				error_code = 'f';
			}
			return ;
		}
		else if(inbuff[i] >= '0' && inbuff[i] <='9'){
			outbuff[j++] = inbuff[i];
			int k = i + 1;
			while(inbuff[k] >= '0' && inbuff[k] <= '9'){
				outbuff[j++] = inbuff[k];
				i = k;
				k++;
			}
			outbuff[j++] = ' ';
		}
		else if(inbuff[i] == '('){
			cpush(stack, top, inbuff[i]);
			(*ocr)++;
		}
		else if(inbuff[i] == ')'){
			(*ccr)++;
			while( ((cscan(stack, top) != '(') && *top != -1 )){
				outbuff[j++] = cpop(stack, top);
			}
			if(cscan(stack, top) == '('){
				cpop(stack, top);
			}

		}
		else if(inbuff[i] == '+' || inbuff[i] == '-'){
			while( ((*top != -1) && (cscan(stack, top) != '(')) ){
				outbuff[j++] = cpop(stack, top);
			}
			cpush(stack, top, inbuff[i]);
		}
		else if(inbuff[i] == '*' || inbuff[i] == '/'){
			while( *top != -1 && cscan(stack, top) != '(' && (cscan(stack, top) == '*' || cscan(stack, top) == '/' )){
				outbuff[j++] = cpop(stack, top);
			}
			cpush(stack, top, inbuff[i]);
		}
	}	
}

int back_calcul(char *outbuff, int *stack, int *top){
	*top = -1;
	for(int i = 0; i < MAX; i++){
		if(outbuff[i] == '='){
			return stack[*top];
		}
		else if(outbuff[i] >= '0' && outbuff[i] <= '9'){
			int temp = outbuff[i] - '0';
			int j = i + 1;
			while(outbuff[j] >= '0' && outbuff[j] <= '9'){
				temp = (temp * 10) + (outbuff[j] - '0');
				i = j;
				j++;
			}
			ipush(stack, top, temp);
		}
		else if(outbuff[i] == '+'){
			int rear = ipop(stack, top);
			int front = ipop(stack, top);
			ipush(stack, top, front + rear);
		}
		else if(outbuff[i] == '-'){
			int rear = ipop(stack, top);
			int front = ipop(stack, top);
			ipush(stack, top, front - rear);
		}
		else if(outbuff[i] == '*'){
			int rear = ipop(stack,top);
			int front = ipop(stack, top);
			ipush(stack, top, front * rear);
		}
		else if(outbuff[i] == '/'){
			int rear = ipop(stack, top);
			int front = ipop(stack, top);
			if(rear == 0){ error_code = 'e'; return 0;}
			ipush(stack, top, front / rear);
		}

	}
}

char cpop(char *stack, int *top){
	if(*top <= -1){ error_code = 'a'; return 0;}
	else{ return stack[(*top)--]; }
}

void cpush(char *stack, int *top, char a){
	if(*top >= MAX){ error_code = 'b'; }
	else{ stack[++(*top)] = a;}
}

char cscan(char *stack, int *top){
	return stack[*top];
}

int ipop(int *stack, int *top){
	if(*top <= -1){ error_code = 'c'; return 0;}
	else{ return stack[(*top)--]; }
}

void ipush(int *stack, int *top, int a){
	if(*top >= MAX){ error_code = 'd'; }
	else{ stack[++(*top)] = a;}
}

int iscan(int *stack, int *top){
	return stack[*top];
}

int main(void){
	char *inbuff;
	char (*queue)[MAX] = (char (*)[MAX])malloc(sizeof(*queue) * QUEUE_MAX);
	char *outbuff = (char *)malloc(sizeof(char) * MAX);
	char *c_stack = (char *)malloc(sizeof(char) * MAX);
	int *i_stack = (int *)malloc(sizeof(int) * MAX);
	int top = -1;
	int ccr = 0;
	int ocr = 0;
	int answer = 0;

	enqueue(queue);
	inbuff = dequeue(queue);

	if(inbuff != NULL){
		back_change(inbuff, outbuff, c_stack, &top, &ccr, &ocr);
		answer = back_calcul(outbuff, i_stack, &top);
	}
	
	if(error_code == '0'){
		printf("answer = %d",answer);
	}else{
		printf("error_code = %c", error_code);
	}

    free(queue);
	free(outbuff);
	free(c_stack);
	free(i_stack);
	return 0;
}
