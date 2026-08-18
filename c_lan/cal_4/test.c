
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 100
#define Q_MAX 10

char error_code = '0';
int q_front = 0;
int q_rear = 0;
int q_count = 0;

void enqueue(char queue[][MAX]);
char* dequeue(char queue[][MAX]);

char cpop(char* stack, int* top);
void cpush(char* stack, int* top, char a);
char cscan(char* stack, int* top);

int ipop(int* stack, int* top);
void ipush(int* stack, int* top, int a);

void back_change(char* inbuff, char* outbuff, char* stack, int* ccr, int* ocr, int* top);
int back_calcul(char* outbuff, int* stack, int* top);

char* dequeue(char queue[][MAX]) {
	if (q_count == 0) {
		error_code = 'd';
		return 0;
	}
	else {
		char* inbuff = queue[q_front];
		q_front = (q_front + 1) % Q_MAX;
		q_count--;
		return inbuff;
	}
}

char cpop(char* stack, int* top) {
	if (*top < 0) { error_code = 'a'; return 0; }
	else { return stack[(*top)--]; }
}

void cpush(char* stack, int* top, char a) {
	if (*top >= MAX) { error_code = 'b';}
	else { stack[++(*top)] = a; }
}

char cscan(char* stack, int* top) {
	return stack[*top];
}

int ipop(int* stack, int* top) {
	if (*top < 0) { error_code = 'a'; return 0; }
	else { return stack[(*top)--]; }
}

void ipush(int* stack, int* top, int a) {
	if (*top >= MAX) { error_code = 'b'; }
	else { stack[++(*top)] = a; }
}

void enqueue(char queue[][MAX]) {
	if (q_count >= Q_MAX) {
		error_code = 'c';
		return;
	}
	else {
		scanf("%s", queue[q_rear]);
		q_rear = (q_rear + 1) % Q_MAX;
		q_count++;
	}
}

void back_change(char* inbuff, char* outbuff, char* stack,int* ccr, int* ocr, int* top){
	*top = -1;
	*ccr = 0;
	*ocr = 0;
	int j = 0;
	for (int i = 0; i < MAX; i++) {
		if (inbuff[i] == '=' || inbuff[i] == '\n' || inbuff[i] == '\0') {
			while (!(*top == -1)) {
				outbuff[j++] = cpop(stack, top);
			}
			outbuff[j] = '=';
			if (*ocr != *ccr) {
				error_code = 'f';
			}
			return;
		}
		else if (inbuff[i] >= '0' && inbuff[i] <= '9') {
			outbuff[j++] = inbuff[i];
			int k = i + 1;
			while (inbuff[k] >= '0' && inbuff[k] <= '9') {
				outbuff[j++] = inbuff[k];
				i = k;
				k++;
			}
			outbuff[j++] = ' ';
		}
		else if (inbuff[i] == '(') {
			cpush(stack, top, inbuff[i]);
			(*ocr)++;
		}
		else if (inbuff[i] == ')') {
			(*ccr)++;
			while (*top != -1 && cscan(stack, top) != '(') {
				outbuff[j++] = cpop(stack, top);
			}
			if (cscan(stack, top) == '(') {
				cpop(stack, top);
			}

		}
		else if (inbuff[i] == '+' || inbuff[i] == '-') {
			while (((*top != -1) && (cscan(stack, top) != '('))) {
				outbuff[j++] = cpop(stack, top);
			}
			cpush(stack, top, inbuff[i]);
		}
		else if (inbuff[i] == '*' || inbuff[i] == '/') {
			while (*top != -1 && cscan(stack, top) != '(' && (cscan(stack, top) == '*' || cscan(stack, top) == '/')) {
				outbuff[j++] = cpop(stack, top);
			}
			cpush(stack, top, inbuff[i]);
		}
	}
}

int back_calcul(char* outbuff, int* stack, int* top) {
	int answer; int rear; int front;
	for (int i = 0; i < MAX; i++) {
		if (outbuff[i] == '=') {
			answer = ipop(stack, top); 
			return answer;
		}
		else if (outbuff[i] >= '0' && outbuff[i] <= '9') {
			int temp = outbuff[i] - '0';
			int k = i + 1;
			while (outbuff[k] >= '0' && outbuff[k] <= '9') {
				temp = (temp * 10) + (outbuff[k++] - '0');
				i = k;
			}
			ipush(stack, top, temp);
		}
		else if (outbuff[i] == '+') {
			int rear = ipop(stack, top);
			int front = ipop(stack, top);
			ipush(stack, top, front + rear);
		}
		else if (outbuff[i] == '-') {
			int rear = ipop(stack, top);
			int front = ipop(stack, top);
			ipush(stack, top, front - rear);
		}
		else if (outbuff[i] == '*') {
			int rear = ipop(stack, top);
			int front = ipop(stack, top);
			ipush(stack, top, front * rear);
		}
		else if (outbuff[i] == '/') {
			int rear = ipop(stack, top);
			int front = ipop(stack, top);
			if (rear == 0) { error_code = 'f'; return 0; }
			ipush(stack, top, front / rear);
		}

	}
}


int main(void) {
	char* inbuff;
	char (*queue)[MAX] = (char (*)[MAX])malloc(sizeof(*queue) * Q_MAX);
	char* outbuff = (char*)malloc(sizeof(char) * MAX);
	char* c_stack = (char*)malloc(sizeof(char) * MAX);
	int* i_stack = (int*)malloc(sizeof(int) * MAX);
	int c_top = -1; int i_top = -1;
	int ccr = 0;
	int ocr = 0;
	int answer = 0;

	enqueue(queue);
	inbuff = dequeue(queue);

	if (strcmp(inbuff, "exit") == 0) {
		printf("프로그램이 종료되었습니다 !!!!!");
		free(outbuff);
		free(queue);
		free(c_stack);
		free(i_stack);
		return 0;
	}
	else {
		back_change(inbuff, outbuff, c_stack, &ccr, &ocr, &c_top);
		answer = back_calcul(outbuff, i_stack, &i_top);
	}


	if (error_code == '0') {
		printf("answer = %d", answer);
	}
	else {
		printf("%c", error_code);
	}

	free(outbuff);
	free(queue);
	free(c_stack);
	free(i_stack);
	return 0;
}

