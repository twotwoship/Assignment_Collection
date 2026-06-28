#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define MAX 101

int solution(const char* babbling[], size_t babbling_len);
int solution_2(const char* test);
void aya_strcmp(const char* test, int *p_which_one, int *p_aya_answer);
void ye_strcmp(const char* test, int *p_which_one, int *p_ye_answer);
void woo_strcmp(const char* test, int *p_which_one, int *p_woo_answer);
void ma_strcmp(const char* test, int *p_which_one, int *p_ma_answer);




int main(void){

	int answer = 0;
	const char *example[] = {
		"ayaye", "uuuma", "ye", "yemawoo", "ayaa"
	};
	const char** p_example = example;	
	size_t example_len = 5;

	answer = solution(p_example, example_len);

	printf("answer = %d\n",answer);
			
	return 0;
}
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
//			printf("babbling : %10s answer = %d i = %d \n",test, answer, i);
        }
    }
    return answer;
}

int solution_2(const char* test){ //babbing 을 하나 가져와야됨. strcmp(a,b) a와 b를 비교햇을 때 같으면 0임
// 여기서 test가 비교해야할 대상은 aya ye woo ma
// 하나씩 비교 해서 문자열을 끝까지 읽었을때 같으면 0을 내보내기.
// test[0]부터 aya ye woo ma 전부 비교해보고 만약 비교했을때 일치한다면 pass 일치하지 않는다면 넘기는거지뭐.
// 처음부터 생각을 잘못했음 들어오는 단어가 네가지 발음을 최대 한번씩 사용해 조합한 발음 밖에 못하는거임.
// 아직 구현 안된거  1. 모든 단어는 최대 한번씩 쓸 수 있다.
	// 2. 네가지 발음 외 다른 것이 나오면 answer = 1이 되어야 한다.

    int answer = 0; int aya_answer = 0; int ye_answer = 0; int woo_answer = 0; int ma_answer = 0;
    int *p_answer = &answer; int *p_aya_answer = &aya_answer; int *p_ye_answer = &ye_answer; 
    int *p_woo_answer = &woo_answer; int *p_ma_answer = &ma_answer;
    int which_one = 0; int *p_which_one = &which_one;
	int using_aya = 0; int using_ye = 0; int using_woo = 0; int using_ma = 0;
    while(test[which_one] != '\0' && answer == 0 && *p_which_one < strlen(test)){ 
	    aya_answer = 0;
		ye_answer = 0;
	    woo_answer = 0;
	    ma_answer = 0;
		//printf("solution_2_while_in \n");
		// test의 which_one 문자가 NULL이 아니고 answer이 0이 아니라면 계속 돌려.
        // 어차피 중간에 answer가 0이 아니게 된다면 while이 종료되고 함수는 그대로 종료.
        // 생각해봐야 할게 저 네 개 중 1개만이라도 같기만 하면 되니까.
        // 어떻게 비교를 해볼까? 4개다 돌렸을 때 같은게 없으면 whlie 문을 탈출시켜야됨.
        // 그럼 4개다 비교를해보고 4개 중 하나라도 strcmp가 0이 없으면 안되는 거임.
		if(using_aya == 0){
        aya_strcmp(test, p_which_one, p_aya_answer);
			if(aya_answer == 1){	using_aya++;	}
		}
		//	printf("which_one = %d \n", which_one);
		if(using_ye == 0){
		ye_strcmp(test, p_which_one, p_ye_answer);
			if(ye_answer == 1){		using_ye++;	}
		}
    	//	printf("which_one = %d ye_answer = %d \n", which_one, ye_answer);
		if(using_woo == 0){
		woo_strcmp(test, p_which_one, p_woo_answer);
			if(woo_answer == 1){	using_woo++;	}
		}
		if(using_ma == 0){
        ma_strcmp(test, p_which_one, p_ma_answer);
			if(ma_answer == 1){		using_ma++;	}
		}
        if(!(aya_answer == 1 || ye_answer == 1 || woo_answer == 1 || ma_answer == 1)){// 첫번째부터 비교했을 때 똑같은게 하나도 없어.
			// 아니지 어차피 처음부터 읽엇을 때 발음 못하는 단어가나오면 그건 못읽는거임.
	//		printf("no answer \n");
            answer = 1;	
        }
//		printf("\n");
    }
    return answer;
}

void aya_strcmp(const char* test, int *p_which_one, int *p_aya_answer){ // 만약에 다르면 다시 빼야됨.
//	printf("test = %s which_one = %d \n", test, *p_which_one);
    char aya[4] = "aya";

	int backup = *p_which_one;

	*p_aya_answer = 0;

	for(int i = 0; i < 3; i++){
		if(test[*p_which_one] == aya[i]){
			(*p_which_one)++;
		}else{
			*p_which_one = backup;
			return;
		}	
	}
	//무사히 순회하면
	*p_aya_answer = 1;
	
}

void ye_strcmp(const char* test, int *p_which_one, int *p_ye_answer){
    char aya[3] = "ye";

	int backup = *p_which_one;

	*p_ye_answer = 0;

	for(int i = 0; i < 2; i++){
		if(test[*p_which_one] == aya[i]){
			(*p_which_one)++;
		}else{
			*p_which_one = backup;
			return;
		}	
	}
	//무사히 순회하면
	*p_ye_answer = 1;
	
}

void woo_strcmp(const char* test, int *p_which_one, int *p_woo_answer){
    char aya[4] = "woo";

	int backup = *p_which_one;

	*p_woo_answer = 0;

	for(int i = 0; i < 3; i++){
		if(test[*p_which_one] == aya[i]){
			(*p_which_one)++;
		}else{
			*p_which_one = backup;
			return;
		}	
	}
	//무사히 순회하면
	*p_woo_answer = 1;
	
}

void ma_strcmp(const char* test, int *p_which_one, int *p_ma_answer){
    char aya[3] = "ma";

	int backup = *p_which_one;

	*p_ma_answer = 0;

	for(int i = 0; i < 2; i++){
		if(test[*p_which_one] == aya[i]){
			(*p_which_one)++;
		}else{
			*p_which_one = backup;
			return;
		}	
	}
	//무사히 순회하면
	*p_ma_answer = 1;
	
}

