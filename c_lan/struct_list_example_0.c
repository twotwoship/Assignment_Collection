#include<stdio.h>

struct list{
	int answer;
	struct list *next;
};

int main(void){
	struct list a = { 10, }, b = { 30, }, c = { 50, };
	struct list *head = &a, *current;

	a.next = &b; b.next = &c;

	printf("%d\n",head->answer);
	printf("%d\n",head->next->answer);
	printf("%d\n",head->next->next->answer);
	
	current = head;
	while(current != NULL){
		printf("33333    %d\n",current->answer);
		current = current->next;
	}
	return 0;
}
