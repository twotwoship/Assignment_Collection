#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void _push(char *s, int *p, char c){
    (*p)++;
    s[*p] = c;
}

void _pop(char *s, int *p){
    s[*p] = '\0';
    (*p)--;
}
// 파라미터로 주어지는 문자열은 const로 주어집니다. 변경하려면 문자열을 복사해서 사용하세요.
bool solution(const char* s) {
    bool answer = true;
    int open = 0;
    char stack[100001] = { 0 };
    int _top = -1;
    int *_pt = &_top;
    int i = 0;
    int check = 0;
    // 괄호가 아닐 경우 탈출!
    while(s[i] == '(' || s[i] == ')'){
        
        // 열린괄호면 그냥 사정없이 스택에 집어넣어.
        if(s[i] == '('){
            _push(stack, _pt, '(');
        // 닫힌괄호면 일단 스택이 비엇는지 안비었는지 확인하고 하나 팝해. 어차피 스택에는 ( 밖에 안들어잇음 애초에 )은 안넣으니까.
        }else if(s[i] == ')'){
            // 스택 비엇는지 확인.
            if(_top == -1){
                // 비어있으면 바로 아웃이야.
                answer = false;
                printf("top is -1\n");
                return answer;
                // 스택이 안비어 있다면?
            }else{
                // 하나 팝해.
                _pop(stack, _pt);
            }

        }
        i++;
    }
    // 다 계산했는데 스택에 뭔가 있다? 그럼 뭔가 빼먹은거야
    if(_top != -1){
        answer = false;
    }
    return answer;
}

int main(void){
    char s[100000] = "(())()";

    int result = solution(s);
    printf("%d\n", result);

    return 0;
}