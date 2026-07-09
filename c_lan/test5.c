#include<stdio.h>
#include<stdlib.h>

typedef struct list{
	int data;
	struct list *prev;
	struct list *next;
} t_list;

typedef struct list_db{
	t_list *head;
	t_list *tail;
	t_list *point;
	int list_stack;
} t_list_db;


void list_add(t_list_db *list, int task);
void list_print(t_list_db *list);
void list_sub(t_list_db *list, int task);
int main(void){
	
	t_list_db *list_1 = (t_list_db *)malloc(sizeof(t_list_db));
	list_1->head = NULL;
	list_1->tail = NULL;
	list_1->point = NULL;

	list_add(list_1, 1);
	list_add(list_1, 2);
	list_add(list_1, 3);
	list_add(list_1, 4);
	list_add(list_1, 5);
	list_add(list_1, 6);	
	printf("stack = %d \n", list_1->list_stack);

	list_print(list_1);
	list_sub(list_1, 3);
	list_print(list_1);
	return 0;
}

void list_add(t_list_db *list, int task){
	t_list *temp = (t_list *)malloc(sizeof(t_list));

	list->list_stack++;
	if(list->head == NULL){
		list->head = temp;
	}

	if(list->tail == NULL){
		list->tail = temp;
	}else{
		temp->prev = list->tail;
		list->tail->next = temp;
		list->tail = temp;
	}
	temp->data = task;
	temp->next = NULL;
}

void list_print(t_list_db *list){
	list->point = list->head;
	while(list->point->next){
		printf("%d   ",list->point->data);
		list->point = list->point->next;
	}
	printf("%d   ",list->point->data);
}

void list_sub(t_list_db *list, int task){
	list->point = list->head;
	while(1){
		if(list->point->data == task){
			printf("remove : %d",list->point->data);
			if(list->point == list->head){
				list->point->next->prev = NULL;
				list->head = list->point->next;
			}else if(list->point == list->tail){
				list->tail = list->point->prev;
				list->point->prev = NULL;
			}else{
				list->point->prev->next = list->point->next;
				list->point->next->prev = list->point->prev;
			}
			break;
		}else{
			list->point = list->point->next;	
		}
	}

}
