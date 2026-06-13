#define MAX_E 100
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // atoi, itoa ..........

typedef struct node* treepointer;
typedef struct {
	int data;
} node;
node heap[MAX_E + 1] = { 0 };
int size = 0;
int check_number = 0;

void push(temp);
node pop();
int binary_search(int temp, int *answer);

int binary_search(int temp, int* answer) {
	// 찾으면    원하는 data 22를 찾았습니다
	// 못찾으면     99를 찾을 수 없습니다.
	// 탐색을 끝냈는데 배열안에 없으면 못찾는거지.
	// 체크넘버까지 지.
	int low, mid, high, low_index, mid_index, high_index;
	int search_result = NULL;
	int stand = check_number-1;
	//printf("stand %d check_number %d :\n", stand, check_number);
	low = answer[0];
	low_index = 0;
	high = answer[stand];
	high_index = stand;
	stand /= 2;
	mid = answer[stand];
	mid_index = stand;
	while (low != high) {
		if (mid == temp) { // 중앙값이 찾고자 하는 값(temp)과 같을때
			//printf("mid = temp \n");
			search_result = mid;
			break;
		}
		if (mid > temp) {// 중앙값이 찾고자 하는 값(temp)보다 크면 왼쪽으로 가야겟지? 그니까 하이 바꾸기.
			//printf("mid = %d > high  = %d\n", mid, high);
			high = mid; // mid와 하이의 일치
			high_index = mid_index;
			//그럼 이제 미드는?
			mid_index /= 2;
			mid = answer[mid_index];
			if (mid == temp) { // 중앙값이 찾고자 하는 값(temp)과 같을때
				search_result = mid;
				break;
			}
			if (low == high) {
				break;
			}
		}
		if (mid < temp) { // 중앙값이 찾고자 하는 값(temp)보다 작을때 오른쪽으로 가야겠지? 로우 바꾸기
			//printf("mid  = %d < high = %d \n", mid, high);
			//printf("low_index = %d mid_index  = %d  high_index = %d \n", low_index, mid_index, high_index);
			low = mid;
			low_index = mid_index; 
			mid_index = (low_index + high_index) / 2;
			if (low_index == mid_index) {
				mid_index++;
			}
			mid = answer[mid_index];
			if (mid == temp) { // 중앙값이 찾고자 하는 값(temp)과 같을때
				search_result = mid;
				break;
			}
			if (low == high) {
				break;
			}
		}
	}
	if (search_result == NULL) {
		printf("%d를 찾을 수 없습니다.\n", temp);
	}
	else {
		printf("원하는 data %d를 찾았습니다.\n", search_result);
	}

	return search_result;
}
void push(temp) {
	int check;
	if (size == MAX_E) {	// 예외처리
		printf("heap is full \n");
		exit(0);
	}
	check = ++size;		//전체 사이즈 하나 늘려
	while ((check != 1) && (temp > heap[check / 2].data)) {		//처음 넣는게 아니고 맨 뒤에보다 크다.
		heap[check] = heap[check / 2];							//앞에 친구 뒤로 보내기
		check /= 2;												//나누고 순회
	}
	node* item = (node*)calloc(1, sizeof(node));
	item->data = temp;
	heap[check] = *item;
}

node pop() {
	int parent, child;
	node data, temp;
	if (size == 0) {	// 예외처리
		printf("heap is empty \n");
		exit(0);
	}
	data = heap[1];	// 1번부터시작해서 1번이 제일 큰놈임 최대힙이라. 1번 뽑고나서 재정렬 필요.
	temp = heap[size--]; // 최대 히프 제일 뒤친구하고 머리하나 뺐으니까 전체 크기 줄여.
	parent = 1;		// 부모 시작 정의
	child = 2;		// 자식 시작 정의
	while (child <= size) {	// 자식이 범위 안에 있을 때
		if ((child < size) && (heap[child].data < heap[child + 1].data)) {	//범위 안이고 and 오른쪽이 자식이 더 크면
			child++;														//자식이 오른쪽임.
		}
		if (temp.data >= heap[child].data) {	//최대 히프 제일 뒤에 친구가 자식친구보다 크거나 같으면 멈추기.
			break;								//왜냐면 맨뒤에놈 뽑았는데 자식보다 크면은 118번째 줄처럼 부모로 올려야되서.
		}
		heap[parent] = heap[child];				//자식을 부모로 끌어올리고 
		parent = child;							//자식이 올라왔으니 부모되고
		child *= 2;								//자식 곱하기 2 // 트리를 생각해라
	}
	heap[parent] = temp;						// 올리기
	return data;
}

int main(void) {
	char buff[40];
	int arr[100];
	int temp = 0;
	//int answer[MAX_E + 1];
	int* answer = calloc(MAX_E + 1, sizeof(int));
	int arr_temp = 0;

	while (1) {
		printf("enter data(종료:exit) : ");
		fgets(buff, 40, stdin);
		if (strncmp(buff, "exit", 4) == 0) {
			break;
		}
		else {
			temp = atoi(buff);
			arr[arr_temp++] = temp;
			push(temp);
		}
	}
	printf("**** input data ****\n");
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	check_number = size;
	printf("**** sort data ****\n");
	for (int i = 0; i < check_number; i++) {
		node temp = pop();
		int tt = temp.data;
		answer[size] = tt;
	}
	for (int i = 0; i < check_number; i++) {
		printf("%d ", answer[i]);
	}
	printf("\n");
	while (1) {
		printf("찾고자 하는 data는 ?(전체: all, 종료:exit) : ");
		fgets(buff, 40, stdin);
		if (strncmp(buff, "exit", 4) == 0) {
			printf("프로그램이 종료 되었습니다 !!!!!!!");
			break;
		}
		else if(strncmp(buff, "all", 3) == 0){
			for (int i = 0; i < check_number; i++) {
				printf("%d ", answer[i]);
			}
			printf("\n");
		}
		else {
			temp = atoi(buff);
			binary_search(temp, answer);
		}
	}
	return 0;
}
