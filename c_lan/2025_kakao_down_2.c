#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// https://school.programmers.co.kr/learn/courses/30/lessons/468377
// 프로그래머스 힌트 스테이지

int case_solution(int case_number, int** cost, size_t cost_rows, size_t cost_cols, int** hint, size_t hint_rows, size_t hint_cols);
// cost_rows는 2차원 배열 cost의 행 길이, cost_cols는 2차원 배열 cost의 열 길이입니다.
// hint_rows는 2차원 배열 hint의 행 길이, hint_cols는 2차원 배열 hint의 열 길이입니다.
int solution(int** cost, size_t cost_rows, size_t cost_cols, int** hint, size_t hint_rows, size_t hint_cols) {
    int answer = 0;
    int case_number = 1; // 경우의 수
    // 힌트 행과 열이 있는데 힌트 행만큼 0000 인걸로해서 모든 경우의 수를 세자. 넘겨주는건 뭘넘겨줄까?
    // 행의 길이만큼 2^승인거니까
    for(int i = 1; i <= hint_rows; i++){
        case_number = case_number * 2; //모든 경우의 수 계산
    }

    // 경우의 수 값이 들어갈 인트형 배열 생산.
    int * _case = (int *)malloc(sizeof(int) * case_number);

    for(int i = 0; i < case_number; i++){
        int k = i + 1;
        _case[i] = case_solution(k, cost, cost_rows, cost_cols, hint, hint_rows, hint_cols); // 반환해서 나온 비용을 배열에 넣는다.
    }

    // 최종 값 계산.
    _case[0] = answer;
    for(int i = 0; i < case_number; i++){
        if(_case[i] < answer){
            answer = _case[i];
        }
    }
    return answer;
}
// k는 1부터 시작해서 모든 경우의수 끝까지 간다 2진법으로 생각해서 1인 것만 사용하는 힌트 번들이다
// 반환하는 값은 경우의 수를 확인해서 맞는 힌트 번들만 사용해서 나온 최종 비용을 반환해준다.
int case_solution(int case_number, int** cost, size_t cost_rows, size_t cost_cols, int** hint, size_t hint_rows, size_t hint_cols){
    int value;
    // 만약 case_number가 1이라면 00001 이니까 사용한 힌트 번들은 1임. 몇번 미는 것은 어떻게 계산하지?
    // 13이라면 01101 제한사항있잖아 힌트 길이가 최대 20이니까 10100 보다 클수가 없음 0x10 하고 비교해서 총 5번만 밀면 된다.
    // 스테이지별 힌트권 사용수를 저장해야겟네?
    //int using_hint[cost_rows] = { 0 }; // 1이 1번 스테이지임 어차피 1번스테이지는 힌트권 없음. 0번은 그냥쓰지마헷갈려 스테이지수만큼 배열 만들기
    int *using_hint = (int *)malloc(sizeof(int) * cost_rows);
    for(int i = 0; i < 5; i++){// 거꾸로 생각해야된다 0이면 5번 번들 1이
        int _real;
        if(i == 0){ _real = 5;}else if(i == 1){ _real = 4;}else if(i == 2){ _real = 3;}else if(i == 3){ _real = 4;}else if(i == 4){ _real = 5;}
        if(case_number & (0x10 >> i)){
            value += hint[i][0]; // 이게 번들임 그 힌트가 들어 있는 번들 hint[i][0] 은 가격임  번들 삿으니까 가격 추가
            for(int j = 1; j < hint_cols; j++){
                // hint 열만큼 수행을 한다.
                // 0 = 5, 1 = 4, 2 = 3, 3 = 2, 4 = 1, 5 = 0 번째 번들
                using_hint[hint[_real][j]]++; // 힌트권 번들 안의 힌트권의 사용가능한 사용횟수를 저장한다.
            }
        }else{
            // 밀었을때 0이면 안쓰는 번들
            // 신경안써도된다.
        }
    }
    // 사용하는 힌트권 번들 다 셋으니까 using_hint배열의 1번부터 가지고 있는 사용가능한 힌트권 개수 만큼 스테이지 비용을 줄여서 value에 더한다.
    for(int i = 0; i < cost_rows ; i++){ // 스테이지 하나씩 봅시다.
        int stage_hint_num = using_hint[i]; // 이게 각 스테이지별 가지고 있는 힌트권의 수. i 번째 스테이지의 힌트권 수인거임.
        value += cost[i][stage_hint_num];
    }

    return value;
}