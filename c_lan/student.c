#include<stdio.h>
#include<stdlib.h>
#define STUDENT_NUMBER 2
typedef struct
{
	int kor;
	int eng;
	int math;
	int sum;
	float avg;
	char* rank;
	char* name;
} t_grade;

typedef struct
{
	int number;
	t_grade* score;
} t_students;
void grade_sort(t_students* student);
void rank_measurement();

void grade_sort(t_students* student) {
	//넘어온걸 정렬해줘야됨 어떻게 정렬하냐? avg가 높은 순서대로 위에서 아래로 다 일일히 하나하나 바꿔줘야됨.
	//너무 빡세니까 점수표 순위를 따로만들어서 거기만 바꾸자. 앞에만 가리키게 넣으면 된다.어차피 뒤는 따라감.
	
	for(int i = 0; i < STUDENT_NUMBER - 1; i++) {

	}
	t_students temp;
	for (int i = 0; i < STUDENT_NUMBER - 1; i++) {
		int j = i + 1;
		for (j; j < STUDENT_NUMBER; j++) { // 앞에놈 뒤에놈 비교 필요 비교하고나서는 어떻게 집어 넣을 생각이지?
			// 배열 가리키는 대가리만 어케 바꾸면 될 것 같은데...
			if((student + i)->score->avg < (student + j)->score->avg){
				// student안에 들어가는 주소 값만 바꿔주면 된다. 어케? 아래 같은 느낌으로?
				temp = student[i];
				student[i] = student[j];
				student[j] = temp;
			}
		}
	}
}
int main(void) {
	t_students* student = (t_students*)malloc(STUDENT_NUMBER * sizeof(t_students));
	for (int i = 0; i < STUDENT_NUMBER; i++) {
		(student + i)->score = (t_grade*)malloc(sizeof(t_grade));
		(student + i)->score->name = (char*)malloc(sizeof(char));
		(student + i)->score->rank = (char*)malloc(sizeof(char));
		printf("학번 : ");
		scanf("%d", &(student + i)->number);
		printf("이름 : ");
		scanf("%s", (student + i)->score->name);
		printf("국어, 영어, 수학 점수 : ");
		scanf("%d %d %d", &(student + i)->score->kor, &(student + i)->score->eng, &(student + i)->score->math );
	}
	for (int i = 0; i < STUDENT_NUMBER; i++) {
		(student + i)->score->sum = (student + i)->score->kor + (student + i)->score->eng + (student + i)->score->math;
		(student + i)->score->avg = (float)((student + i)->score->sum / 3);
		if ((student + i)->score->avg >= 90) {
			*(student + i)->score->rank = 65;
		}
		else if ((student + i)->score->avg >= 80) {
			*(student + i)->score->rank = 65;
		}
		else if ((student + i)->score->avg >= 70) {
			*(student + i)->score->rank = 65;
		}
		else {
			*(student + i)->score->rank = 65;
		}
	}
	printf("# 정렬 전 데이터...\n");
	for (int i = 0; i < STUDENT_NUMBER; i++) {
		printf("%5d %7s %4d %4d %4d %5d %4.1f 랭크 : %s\n", (student + i)->number, (student + i)->score->name, 
			(student + i)->score->kor, (student + i)->score->eng, (student + i)->score->math, (student + i)->score->sum, 
			(student + i)->score->avg, (student + i)->score->rank);
	}
	grade_sort(student);

	printf("# 정렬 후 데이터...\n");
	for (int i = 0; i < STUDENT_NUMBER; i++) {
		printf("%5d %7s %4d %4d %4d %5d %4.1f %s\n", (student + i)->number, (student + i)->score->name,
			(student + i)->score->kor, (student + i)->score->eng, (student + i)->score->math, (student + i)->score->sum,
			(student + i)->score->avg, (student + i)->score->rank);
	}
	return 0;
}
