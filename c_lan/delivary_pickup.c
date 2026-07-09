#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// deliveries_len은 배열 deliveries의 길이입니다.
// pickups_len은 배열 pickups의 길이입니다.
// cap = 최대 적재량, n 배달할 집의 수, deliveries 집별 배달 수량이 담긴 배열, pickups 집별 수거 수량이 담긴 배열
// 모든 집은 물류창고로부터 i번째만큼 거리가 떨어져 있음.
// 가장 적게 움직이는 방법은 최대 적재량과 관계 없이. 한번 운송에 가장 먼 곳을 제일 먼저 클리어해야됨 
// 출발하기전에 짐을 얼마나 실을것인지 어떻게 판단하냐.
// 아 일단 배송 끝집을 보고 배달과 수거를 판단해 어떻게 판단하냐, 배달이 먼저들어가고 그다음 수거를 들어가,
// 마지막 집의 배달과 수거를 끝내면 work_run 하나 줄여서 더이상 바라보지 않게 만들어.
long long solution(int cap, int n, int deliveries[], size_t deliveries_len, int pickups[], size_t pickups_len) {
    long long answer = -1; // 왜 -1부터 시작? 잘모름 트럭이 움직인 거리.
    answer = 0;
    int work_run = n;
    int truck_trunk = 0;
    int remain_trunk = 0;
    while(work_run >= 1){ // 갈집이 없어질 때까지 돌리기.
        answer = work_run;
        // 배달 먼ㄴ저해
        for(int i = work_run; i >= 0; i--){
            if(cap == truck_trunk){             // 다 짐을 실엇다면?
                break;
            }
            if(cap - truck_trunk >= deliveries[i]){ // 짐칸이 실어야 할 배달보다 많다면
                truck_trunk += deliveries[i];       // 짐칸에 짐 싣고
                deliveries[i] = 0;                  // 배달 집 다 빼고
            }else                                   // 아니다 짐칸보다 배달이 더 많다.
                remain_trunk = cap - truck_trunk;   // 남은 공간 체크하고
                truck_trunk += remain_trunk;        // 남은 공간만큼만 짐 싣고
                deliveries[i] -= remain_trunk;      // 짐 실은 만큼만 배달 집 빼고.
        }
        
        // 배달했으니까 트렁크 비우고
        truck_trunk = 0;    
        remain_trunk = 0;
        
        //이제 수거해
        for(int i = work_run; i >= 0; i--){
            if(cap == truck_trunk){             // 다 짐을 실엇다면?
                break;
            }
            if(cap - truck_trunk >= pickups[i]){ // 짐칸이 실어야 할 수거보다 많다면
                truck_trunk += pickups[i];       // 짐칸에 수거 싣고
                pickups[i] = 0;                  // 수거 집 다 빼고
            }else                                   // 아니다 짐칸보다 수거가 더 많다.
                remain_trunk = cap - truck_trunk;   // 남은 공간 체크하고
                truck_trunk += remain_trunk;        // 남은 공간만큼만 수거 싣고
                pickups[i] -= remain_trunk;      // 짐 실은 만큼만 수거 집 빼고.
        }
        
        
        truck_trunk = 0;
        remain_trunk = 0;
        //while문 맨마지막에 들어가야함 맨 마지막집 배달&수거가 끝났는지 확인.
        if(deliveries[work_run] == 0 && pickups[work_run] == 0){
            work_run--;
        }

    }
    return answer;
}

int main(void){
    int cap = 4;
    int n = 5;
    int deliveries[] = {1, 0, 3, 1, 2};
    size_t deliveries_len = 5;
    int pickups[] = {0, 3, 0, 4, 0};
    size_t pickups_len = 5;
    int answer = solution(cap, n, deliveries, deliveries_len, pickups, pickups_len);

    printf("answer = %d\n", answer);
}