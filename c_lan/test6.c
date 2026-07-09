#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define MAX 101

int solution_2(const char* test);
void aya_strcmp(const char* test, int *p_which_one, int *p_aya_answer);
void ye_strcmp(const char* test, int *p_which_one, int *p_ye_answer);
void woo_strcmp(const char* test, int *p_which_one, int *p_woo_answer);
void ma_strcmp(const char* test, int *p_which_one, int *p_ma_answer);
// babbling_len은 배열 babbling의 길이입니다.
// 파라미터로 주어지는 문자열은 const로 주어집니다. 변경하려면 문자열을 복사해서 사용하세요.
int solution(const char* babbling[], size_t babbling_len) {
    int answer = 0;
    char test[MAX] = { 0 };
    // babbling 문자열 배열인데 1개씩 꺼내서 비교해야됨.
    for(int i = 0; i < babbling_len; i++){
        strcpy(test,babbling[i]);
        if(solution_2(test) == 0){
            answer++;
        }
    }
    return answer;
}

int solution_2(const char* test){ //babbing 을 하나 가져와야됨. strcmp(a,b) a와 b를 비교햇을 때 같으면 0임
// 여기서 test가 비교해야할 대상은 aya ye woo ma
// 하나씩 비교 해서 문자열을 끝까지 읽었을때 같으면 0을 내보내기.
// test[0]부터 aya ye woo ma 전부 비교해보고 만약 비교했을때 일치한다면 pass 일치하지 않는다면 넘기는거지뭐.
    int answer = 0; int aya_answer = 0; int ye_answer = 0; int woo_answer = 0; int ma_answer = 0;
    int *p_answer = &answer; int *p_aya_answer = &aya_answer; int *p_ye_answer = &ye_answer; 
    int *p_woo_answer = &woo_answer; int *p_ma_answer = &ma_answer;
    int which_one = 0; int *p_which_one = &which_one;
    while(test[which_one] != '\0' && answer == 0){ // test의 which_one 문자가 NULL이 아니고 answer이 0이 아니라면 계속 돌려.
        // 어차피 중간에 answer가 0이 아니게 된다면 while이 종료되고 함수는 그대로 종료.
        // 생각해봐야 할게 저 네 개 중 1개만이라도 같기만 하면 되니까.
        // 어떻게 비교를 해볼까? 4개다 돌렸을 때 같은게 없으면 whlie 문을 탈출시켜야됨.
        // 그럼 4개다 비교를해보고 4개 중 하나라도 strcmp가 0이 없으면 안되는 거임.
        aya_strcmp(test, p_which_one, p_aya_answer);
        ye_strcmp(test, p_which_one, p_ye_answer);
        woo_strcmp(test, p_which_one, p_woo_answer);
        ma_strcmp(test, p_which_one, p_ma_answer);
        if(!(aya_answer == 0 || ye_answer == 0 || woo_answer == 0 || ma_answer == 0)){
            answer = 1;
        }
    }
    return answer;
}

void aya_strcmp(const char* test, int *p_which_one, int *p_aya_answer){
    char aya[4] = "aya";
    int compare = 0;
    while(aya[compare] != '\n'){
        if(test[*p_which_one] == aya[compare]){
            printf("%c %c",test[*p_which_one], aya[compare]);
            compare++;
            p_which_one++;
            *p_aya_answer = 0;
        }else{
            *p_aya_answer = 1;
        }   
    }
}
void ye_strcmp(const char* test, int *p_which_one, int *p_ye_answer){
    
}
void woo_strcmp(const char* test, int *p_which_one, int *p_woo_answer){
    
}
void ma_strcmp(const char* test, int *p_which_one, int *p_ma_answer){
    
}
