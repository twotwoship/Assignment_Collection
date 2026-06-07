#define MAX 100
#include <stdio.h>
#include <stdlib.h>
int top = -1;
int i_top = -1;
int ocb = 0; // open_culry_bracket
int ccb = 0; // close_culry_bracket
void back_display_make(char* inbuff, char* temp_buff, char* operater_stack);
void back_display_carculate(char* temp_buff, int* number_stack);
char pop_c(char* operater_stack);
char push_c(char* operater_stack, char* inbuff);
char peek_c(char* operater_stack);
int pop_i(int* number_stack);
int push_i(int* number_stack, int buff);
int peek_i(int* number_stack);

void back_display_make(char* inbuff, char* temp_buff, char* operater_stack) {
	int temp_buff_check = 0;
	for (int i = 0; i < MAX; i++) {
		if (inbuff[i] == 10) { // if inbuff[i] is \0, breaking 
			while (top != -1) { // stack에 남은 연산자 전부 꺼내서 출력하기.
				temp_buff[temp_buff_check++] = pop_c(operater_stack);
			}
			temp_buff[temp_buff_check++] = inbuff[i];
			break;
		}
		else if (inbuff[i] == 32) {
			continue;
		}
		else if (inbuff[i] >= 48 && inbuff[i] <= 57) { // number in
			temp_buff[temp_buff_check++] = inbuff[i];
			if (!(inbuff[i + 1] >= 48 && inbuff[i + 1] <= 57)) { //뒤에가 숫자가 아니면 공백을 하나 넣어줘야됨
				temp_buff[temp_buff_check++] = 32;
			}
		}
		else { //여기서부터는 연산자를 스택에 집어넣고 출력하기
			// 연산자 뭐있냐 (=40  )=41         /=47 *=42 -=45 +=43 더잇나?
			if (inbuff[i] == 40) { // ( 나올때
				push_c(operater_stack, &inbuff[i]); // (를 스택에 푸쉬하기.
				ocb++;
			}
			else if (inbuff[i] == 41) { // ) 나올 떄
				ccb++;
				while (1) { // (이 나올때까지 전부 출력
					// 하나씩 팝하기전에 비어있으면 뭔가 괄호가 잘못 들어간거임.///////////////
					if (ocb < ccb) {
						break;
					}
					char temp = pop_c(operater_stack); // 하나씩 팝해
					if (temp == 40) { // (이 나오면 멈추기
						break;
					}
					temp_buff[temp_buff_check++] = temp; // 꺼낸 것을 버퍼에 출력하기.
				}
			}
			else if (inbuff[i] == 47) { // 나누기면 * - + 전부 우선이라고 생각하면됨.
				if (top == -1) { // 스택이 비어 있을 경우
					push_c(operater_stack, &inbuff[i]);
				}
				else { // 스택이 비어있지 않을 경우 
					//아니지 + - 일때는 스택에 넣는다. 아닐 경우 출력
					if (peek_c(operater_stack) == 43 || peek_c(operater_stack) == 45) {// 더하기나 빼기일 경우
						push_c(operater_stack, &inbuff[i]);
					}
					else if (peek_c(operater_stack) == 40) { // 앞에 (이면 그냥 push해주면됨
						push_c(operater_stack, &inbuff[i]);
					}
					else { // 스택의 상단이 *나 / 이고 (가 아닐때 팝하고 푸쉬
						while (top != -1 && peek_c(operater_stack) != 40 &&
							(peek_c(operater_stack) == 42 || peek_c(operater_stack) == 47)) {
							temp_buff[temp_buff_check++] = pop_c(operater_stack);
						}
						push_c(operater_stack, &inbuff[i]);
					}
				}
			}
			else if (inbuff[i] == 42) { // 곱하기면 * - + 전부 우선이라고 생각하면됨.
				if (top == -1) { // 스택이 비어 있을 경우
					push_c(operater_stack, &inbuff[i]);
				}
				else { // 스택이 비어있지 않을 경우 
					// 스택의 상단이 * / (
					//+ - 일때는 스택에 넣는다. 아닐 경우 출력
					if (peek_c(operater_stack) == 43 || peek_c(operater_stack) == 45) {// 더하기나 빼기일 경우
						push_c(operater_stack, &inbuff[i]);
					}
					else if (peek_c(operater_stack) == 40) { // 앞에 (이면 그냥 push해주면됨
						push_c(operater_stack, &inbuff[i]);
					}
					else { // 스택의 상단이 *나 / 이고 (가 아닐때 팝하고 푸쉬
						while (top != -1 && peek_c(operater_stack) != 40 &&
							(peek_c(operater_stack) == 42 || peek_c(operater_stack) == 47)) {
							temp_buff[temp_buff_check++] = pop_c(operater_stack);
						}
						push_c(operater_stack, &inbuff[i]);
					}
				}
			}
			else if (inbuff[i] == 45) { // 빼기면 더하기 빼기 두개보다 우선순위임.
				if (top == -1) { // 스택이 비어 있을 경우
					push_c(operater_stack, &inbuff[i]);
				}
				else { // 스택이 비어있지 않을 경우
					if (peek_c(operater_stack) == 40) { // 스택의 (면 그냥 push 그외는 전부 스택 최상단을 팝하고 본인을 스택에 넣기
						push_c(operater_stack, &inbuff[i]);
					}
					else { // 스택이 비기전과 (가 나오기 전까지 계속 팝
						while (top != -1 && peek_c(operater_stack) != 40) {
							temp_buff[temp_buff_check++] = pop_c(operater_stack);
						}
						// 하고 +를 푸쉬
						push_c(operater_stack, &inbuff[i]);
					}
				}
			}
			else if (inbuff[i] == 43) { // 더하기면 더하기 빼기 두개보다 우선순위임.
				if (top == -1) { // 스택이 비어 있을 경우
					push_c(operater_stack, &inbuff[i]);
				}
				else { // 스택이 비어있지 않을 경우
					if (peek_c(operater_stack) == 40) { // 스택의 (면 그냥 push 그외는 전부 스택 최상단을 팝하고 본인을 스택에 넣기
						push_c(operater_stack, &inbuff[i]);
					}
					else { // 스택이 비기전과 (가 나오기 전까지 계속 팝
						while (top != -1 && peek_c(operater_stack) != 40) {
							temp_buff[temp_buff_check++] = pop_c(operater_stack);
						}
						// 하고 +를 푸쉬
						push_c(operater_stack, &inbuff[i]);
					}
				}
			}
		}
	}
}
void back_display_carculate(char* temp_buff, int* number_stack) {
	// 배열의 앞에서부터 하나씩 출력해서 확인을해야됨 숫자의 법위붜
	int temp_num = 0;
	for (int i = 0; i < MAX; i++) {
		if (temp_buff[i] == 10) { // if temp_buff[i] is \0, breaking
			break;
		}
		// operater_stack에서 하나씩 꺼내기.
		// 숫자일 경우
		else if (temp_buff[i] >= 48 && temp_buff[i] <= 57) { // input number_list; /아니지 이걸 어차피 그대로 집어넣으면 된다.
			// 여러자리의 수 검증을 위해 뒤에가 수인지 아닌지 확인해서
			// 수라면 임시버퍼에 넣어서 체크 아니면 다음으로 넘어가면됨.
			temp_num = temp_buff[i] - 48;
			int j = i; j++;
			//다음자리도 숫자일 경우에는 
			while ((temp_buff[j] >= 48 && temp_buff[j] <= 57) == 1) { // 0 to 9 beside multi digits
				int rear_num = temp_buff[j] - 48; // rear_number
				temp_num = 10 * temp_num + rear_num;
				i = j;
				j++;
				i++;
			}// 연속된숫자가 나온다면 temp_num 이친구를 num_stack 에 푸쉬한다
			push_i(number_stack, temp_num);
		}
		//숫자가 아닌 연산자일 경우 계산을 해서 다시 num_stack에 푸쉬한다
		else { // + - * / 네가지로 구분해서 넣는다.
			if (temp_buff[i] == 43) { // + 뒤에 들어갈 계산할 정수 먼저 꺼내야됨.
				int rear = pop_i(number_stack);
				int front = pop_i(number_stack);
				push_i(number_stack, front + rear);
			}
			else if (temp_buff[i] == 45) { // -
				int rear = pop_i(number_stack);
				int front = pop_i(number_stack);
				push_i(number_stack, front - rear);
			}
			else if (temp_buff[i] == 42) { // *
				int rear = pop_i(number_stack);
				int front = pop_i(number_stack);
				push_i(number_stack, front * rear);
			}
			else if (temp_buff[i] == 47) { // /
				int rear = pop_i(number_stack);
				int front = pop_i(number_stack);
				push_i(number_stack, front / rear);
			}
		}
	}
}

int pop_i(int* number_stack) {
	int res = 0;
	if (i_top == -1) {
		printf("empty_stack\n");
		exit(1);
	}
	else {
		res = number_stack[i_top];
		number_stack[i_top] = 0;
		i_top--;
	}
	return res;
}
int push_i(int* number_stack, int buff) {
	if (i_top == MAX - 1) {
		printf("full_stack\n");
		exit(1);
	}
	else {
		i_top++;
		number_stack[i_top] = buff;
	}
	return 0;
}
int peek_i(int* number_stack) {
	return number_stack[i_top];
}
char pop_c(char* operater_stack) { //list take out
	//printf("check_pop \n");
	char res = 0;
	if (top == -1) {
		printf("empty_stack\n");
		exit(1);
	}
	else {
		res = operater_stack[top];
		top--;
	}
	return res;
}
char push_c(char* operater_stack, char* inbuff) { //list take in
	//printf("check_push\n");
	if (top == MAX - 1) {
		printf("full_stack\n");
		exit(1);
	}
	else {
		top++;
		operater_stack[top] = *inbuff;
	}
	return 0;
}
char peek_c(char* operater_stack) { //출력하지 않고 최상단만 보기
	//printf("check_peek\n");
	return operater_stack[top];
}

int main(void) {
	char* inbuff = (char*)calloc(MAX, sizeof(char)); // 중위 계산식 입력 버퍼
	char* temp_buff = (char*)calloc(MAX, sizeof(char)); // 후위계산식 만들 때 쓰는 출력버퍼
	int* number_stack = (int*)calloc(MAX, sizeof(int)); // 후위 계산 할 때 쓰는 스택 // 이 스택용 pop push도 만들어야됨.
	char* operater_stack = (char*)calloc(MAX, sizeof(char)); // 후위계산식 만들 때 쓰는 스택
	printf("input : ");
	fgets(inbuff, 100, stdin);

	back_display_make(inbuff, temp_buff, operater_stack); // 후위 계산식 만드는 함수	

	if (ocb != ccb) {
		printf("invalid_input\n");
		return 0;
	}

	back_display_carculate(temp_buff, number_stack); // 후위 계산 함수
	printf("answer = %d\n", pop_i(number_stack));
	return 0;
}
