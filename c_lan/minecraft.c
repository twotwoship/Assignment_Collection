#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// picks_len은 배열 picks의 길이입니다.
// minerals_len은 배열 minerals의 길이입니다.
// 파라미터로 주어지는 문자열은 const로 주어집니다. 변경하려면 문자열을 복사해서 사용하세요.
int solution(int picks[], size_t picks_len, const char* minerals[], size_t minerals_len) {
    int answer = 0;
    // 크게 두 경우로 나뉜다 곡괭이가 더 많거나, 광물이 더많거나
    int sum_pickaxe = picks[0] + picks[1] + picks[2];  //곡괭이의 총 개수
    int temp[15][3] = { 0 };
    if(sum_pickaxe * 5 <= minerals_len){
       // printf("small \n");
        // 광물이 더 많거나 같을 경우. 비중계산해서 돌곡을 사용했을 때 피로도가 가장 적게드는걸 돌곡부터 쓰고
        // 곡괭이의 총 개수 * 5 까지만 캘꺼임.
        // 5단위로 찢어가지고 temp[sum_pickaxe][3(각 곡괭이별 캤을 때 피로도 값.)] 안에 집어넣기
        //temp[i][0] = ;  // 다곡으로 캤을 때 피로도
        //temp[i][1] = ;  // 철곡으로 캤을 때 피로도
        //temp[i][2] = ;  // 돌곡으로 캤을 때 피로도
        int i = 0;
        while(i < sum_pickaxe){ // 총 곡괭이의 개수만큼 광물 5개로 짤라서 계산해야됨.
            // 저게 뭔지 각각 확인해야됨. // 각 곡괭이로 캤을 때 피로도 계산
            // 내가 생각을 잘못했다 아니지 맞다. 
            for(int j = 0; j < 5; j++){
                //printf(" j = %d 뭔 광물임? = %s\n", i*5 + j, minerals[i*5 + j]);
                if(minerals[i*5 + j] == "diamond"){ // 몇번째 광물이 다이아일때 각 곡괭이로 캤을 때 피로도
                    temp[i][0] += 1;
                    temp[i][1] += 5;
                    temp[i][2] += 25;
                }else if(minerals[i*5 + j] == "iron"){ // 철일때 피로도
                    temp[i][0] += 1;
                    temp[i][1] += 1;
                    temp[i][2] += 5;
                }else{                                  // 돌일때 피로도
                    temp[i][0] += 1;
                    temp[i][1] += 1;
                    temp[i][2] += 1;
                }
            }
            i++;
        }
        for(int q = 0; q < sum_pickaxe; q++){
            //printf(" 다이아 = %d 철 = %d 돌 = %d\n", temp[q][0],temp[q][2],temp[q][2]);
        }
        // 피로도 계산 끝났어.     temp[15][3] = { 0 }; // 어차피 비어있는데는 곡괭이가 없는거다.
        // 그다음에 temp[i] 부터 계산해서 [0][1][2] 각각 돌곡괭이부터 사용하게 만들어서 셋중 가장 값이 낮은 놈을 쓰게 만들어서 answer에 집어넣기
        // for문두번 전체를 두번 돌아야됨 어케 간단하게 하는방법 없을까? 
        // 
        for(int k = 0; k < sum_pickaxe; k++){ // 전체 곡괭이 수만큼 반복 어떤곡괭이를 쓸지 먼저 정해야됨여기서 그다음에 그 안 포문에서 고르기
            int select_pickaxe = 0; // 0 다이아 1 쇠 2 돌
            int fatigue = 0;        // 피로도
            int m = 0;
            int m_check = 0;        // 곡괭이 쓴곳
            if(picks[2] > 0){ select_pickaxe = 2; picks[2]--; }else if(picks[1] > 0){ select_pickaxe = 1; picks[1]--; }else if(picks[0] > 0){ select_pickaxe = 0; picks[0]--; } // 돌곡괭이부터 사용하게끔
            fatigue = temp[m][select_pickaxe];
            for(m = 0; m < sum_pickaxe; m++){ // 캐야할 부분 순회해서 가장 효율적으로 곡괭이 배치.
                // select_pickaxe 에 따라서 뭘 볼지 정하기. 가장 값이 적은거골라야됨.
                if(fatigue >= temp[m][select_pickaxe]){
                    fatigue = temp[m][select_pickaxe];
                    m_check = m;
                }
            }
            // 포문끝나면 쓴 곡괭이 하나 빼고 나온 피로도 결과에 집어넣기
            picks[select_pickaxe]--;
            //printf("피로도 : %d, 사용한 곡괭이 = %d, 광물 캔 피로도 = %d, 몇번째 광물라인 캤는지 = %d\n",fatigue, select_pickaxe, temp[m_check][select_pickaxe], m_check);
            answer += fatigue;
            //곡괭이를 쓴 광물집합은 다시 못쓰게 만들어야됨.
            for(int z = 0; z < 3; z++){
                temp[m_check][z] = 999;
            }
        }
    }else{
        // 곡괭이가 더 많을 경우. 비중계산해서 돌곡을 사용했을 때 피로도가 가장 많이 드는걸 다이아 or 철곡부터 쓴다.
    }
    return answer;
}

int main(void){

    int pick[3] = 	{0, 1, 1};
    const char* mine[] =  {"diamond", "diamond", "diamond", "diamond", "diamond", "iron", "iron", "iron", "iron", "iron", "diamond"};
    //int answer = solution(int picks[], size_t picks_len, const char* minerals[], size_t minerals_len);
    int answer = solution(pick, 3, mine, 11);
    printf("answer = %d \n", answer);
    return 0;
}