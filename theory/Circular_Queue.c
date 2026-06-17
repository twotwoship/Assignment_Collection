Circular Queue. 원형 큐

#define QUEUE_SIZE 4
#define COMMAND_LENGTH 80

volatile int rear = 0; // 인터럽트에서 큐에 데이 터 ㅓㅈ장
volatile int front = 0; // pㅊ커맨드프로ㅔ싱에서 가져가는 위치

volatile unsigned char rx_buff[QUEUE_SIZE][COMMAND_LENGTH]

//포화상태에서는 enqueue가 실해되지 않아야함.
//(rear+1)%QUEUE_SIZE(4) == front;


