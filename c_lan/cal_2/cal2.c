#include <stdio.h>
#define MAX 100
// 2026_07_31 시작시간 13:30 종료시간 15:00 
// 다음에 만들때는 정수연산이아니라 double 이나 float으로 만들어서 소숫점 연산도 가능하게 해보자.

char _error_code = 0;

void _back_change(char *in_buff, char *out_buff, char *_char_stack, int *top);
int _back_calcul(char *out_buff, int *_int_stack, int *top);
char _char_pop(char *stack, int *top);
void _char_push(char *stack, int *top, char a);
char _char_scan(char *stack, int *top);
int _int_pop(int *stack, int *top);
void _int_push(int *stack, int *top, int a);

void _back_change(char *in_buff, char *out_buff, char *_stack, int *top){
	// 주의 사항 숫자 후위변환후 반드시 그 다음에 스페이스바 넣어줄 것 하나의 수인지 다음 수인지 구불 할 수 있게
	int i; int o = 0;
	for(i = 0; i < MAX; i++){
		if(in_buff[i] == '\n' || in_buff[i] == '\0'){
			while(!(*top == -1)){
				out_buff[o++] = _char_pop(_stack, top);
			}
			out_buff[o++] = '\n';
			out_buff[o] = '\0';
			return ;
		}
		else if(in_buff[i] >= '0' && in_buff[i] <= '9'){
			while((in_buff[i] >= '0' && in_buff[i] <= '9')){
				out_buff[o++] = in_buff[i++];
			}
			out_buff[o++] = ' ';
			i--;
		}
		// 스택[top]의 연산자 우선순위가 낮다면 push
		// stack[top]의 연산자 우선순위가 같거나 높다면 pop후 재비교
		else if(in_buff[i] == '+' || in_buff[i] == '-'){
			if(*top == -1){
				_char_push(_stack, top, in_buff[i]);
			}
			else if( (_char_scan(_stack,top) == '+') || (_char_scan(_stack,top) == '-') ){	
				while(*top >= 0 && _char_scan(_stack, top) != '('){
					out_buff[o++] = _char_pop(_stack,top);
				}	
				_char_push(_stack, top, in_buff[i]);
			}	
		}
		else if(in_buff[i] == '*' || in_buff[i] == '/'){
			if(*top == -1){
				_char_push(_stack, top, in_buff[i]);
			}
			else if( (_char_scan(_stack,top) == '+') || (_char_scan(_stack,top) == '-') ){
				_char_push(_stack, top, in_buff[i]);
			}
			else if( (_char_scan(_stack,top) == '*') || (_char_scan(_stack,top) == '/') ){	
				while(*top >= 0 && (_char_scan(_stack,top) == '*' || _char_scan(_stack, top) == '/')){
					out_buff[o++] = _char_pop(_stack,top);
				}
				_char_push(_stack, top, in_buff[i]);
			}
		}
		else if(in_buff[i] == '('){
			_char_push(_stack, top, in_buff[i]);
		}
		else if(in_buff[i] == ')'){
			while(*top >= 0 && _char_scan(_stack, top) != '('){
				out_buff[o++] = _char_pop(_stack, top);
			}
			if(*top < 0){
				_error_code = 'e';
				return ;
			}

			_char_pop(_stack, top);
		}
	}
}


int _back_calcul(char *out_buff, int *_stack, int *top){
	int answer;	int i;	int j;	int front;	int rear;	int temp;
	for(i = 0; i < MAX; i++){
		// 맨 마지막
		if(out_buff[i] == '\n'){
			answer = _int_pop(_stack, top);
			return answer;
		}
		// 숫자만 형변환해서 넣어주기
		else if(out_buff[i] >= '0' && out_buff[i] <= '9'){
			temp = out_buff[i] - '0';
			j = i + 1;
			// 0과 9 사이가 아닐때까지 계속 반복 연속된 자리수를 확인.
			while((out_buff[j] >= '0' && out_buff[j] <= '9')){
				temp = temp * 10 + (out_buff[j] - '0');
				j++; i++;
			}
			_int_push(_stack, top, temp);
		}
		// 연산자면 스택꺼내서 비교
		else if(out_buff[i] == '+'){
			int rear = _int_pop(_stack, top);
			int front = _int_pop(_stack, top);
			_int_push(_stack, top, front + rear);
		}
		else if(out_buff[i] == '-'){
			int rear = _int_pop(_stack, top);
			int front = _int_pop(_stack, top);
			_int_push(_stack, top, front - rear);
		}
		else if(out_buff[i] == '*'){
			int rear = _int_pop(_stack, top);
			int front = _int_pop(_stack, top);
			_int_push(_stack, top, front * rear);
		}	
		else if(out_buff[i] == '/'){
			int rear = _int_pop(_stack, top);
			int front = _int_pop(_stack, top);
			_int_push(_stack, top, front / rear);
		}
	}
	return answer;
}


char _char_pop(char *stack, int *top){
	if(*top <= -1){ _error_code = 'a'; return 0;
	}else{ return stack[(*top)--]; }
}

void _char_push(char *stack, int *top, char a){
	if(*top == MAX){ _error_code = 'b'; return;
	}else{ stack[++(*top)] = a; }
}
char _char_scan(char *stack, int *top){
	if(*top <= -1){ _error_code = 'a'; return 0;
	}else{	return stack[*top]; }
}

int _int_pop(int *stack, int *top){
	if(*top <= -1){ _error_code = 'c'; return 0;
	}else{ 	return stack[(*top)--];	}
}

void _int_push(int *stack, int *top, int a){
	if(*top >= MAX){	_error_code = 'd'; return;
	}else{	stack[++(*top)] = a;	}
}

int main(void){
	char in_buff[MAX] = { 0 };	// 함수입력
	char out_buff[MAX] = { 0 };
	char _char_stack[MAX] = { 0 };
	int _int_stack[MAX] = { 0 };
	int _top = -1;
	int answer = 0;
	scanf("%s", in_buff);

	_back_change(in_buff, out_buff, _char_stack, &_top);
	answer = _back_calcul(out_buff, _int_stack, &_top);
	printf("answer = %d\n",answer);

	return 0;
}
