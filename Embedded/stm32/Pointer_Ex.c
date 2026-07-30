/***********************************************************/
// [0] �����Ϸ� �׽�Ʈ
/***********************************************************/

#if 0

#include <stdio.h>

int main(void)
{
	printf("Hello C\n");

	return 0;
}

#endif

/***********************************************************/
// [1] �ּ� Ÿ���� �°�
/***********************************************************/

#if 0

#include <stdio.h>

int main(void)
{
	int *p, int *q;
	
	p = (int *)0x1008;
	q = (int *)0x1000;
	
	printf("%X\n", p * q);
	printf("%X\n", p / q);
	printf("%X\n", p + q);
	printf("%X\n", p - q);
	
	printf("%X\n", p * 2);
	printf("%X\n", p / 2);
	printf("%X\n", p + 2);
	printf("%X\n", p - 2);	

	return 0;	
}

#endif

/***********************************************************/
// [2] �迭 parameter
/***********************************************************/

#if 0

#include <stdio.h>

int sum(int b[4])
{
	int i, sum = 0;

	for(i=0; i<(sizeof(b)/sizeof(b[0])); i++)
	{
		sum += b[i];
	}
	return sum;
}

int main(void)
{
	int a[4] = {1,2,3,4};

	printf("%d\n", sum(a));

	return 0;	
}

#endif

/***********************************************************/
// [3] �迭 Ȱ����� �̿��� �迭 ��� �׼���
/***********************************************************/

#if 0 

#include <stdio.h>

int a[4] = {1,2,3,4};
	
int main(void)
{
	printf("%d\n", a[0]);
	printf("%d\n", a[3]);
	printf("%d\n", a[4]);
	printf("%d\n", a[-1]);

	printf("%d\n", (a+1)[2]);
	printf("%d\n", a[3]);

	printf("%d\n", a[2]);
	printf("%d\n", *(a+2));
	printf("%d\n", *(2+a));
	printf("%d\n", 2[a]);
	
	return 0;	
}

#endif

/**********************************************************/
// [4] : ������ �μ�ɱ��?
/**********************************************************/

#if 0

#include <stdio.h>

int main(void)
{
	char a[4] = "LEW";
	char *p = a;

	printf("%X:%X \n", p, &p);
	printf("%X:%X \n", a, &a);
	printf("%X:%X \n", main, &main);
	printf("%X:%X \n", "LEW", &"LEW");
	
	return 0;	
}

#endif

/**********************************************************/
// [1] : ������ �迭
/**********************************************************/

#if 0

#include <stdio.h> 

int x[4] = {1,2,3,4};

int main(void)
{
	int *a[4] = {x+3, x+2, x+1, x};
	
	printf("%d\n", x[2]);	

	// �迭 a�� �̿��Ͽ� x[2]�� 30����

	     = 30;   

	printf("%d\n", x[2]);
	
	return 0;	
}

#endif

/**********************************************************/
// [6] : ��ġ�� Ȱ��
/**********************************************************/

#if 0

#include <stdio.h>

int x`[4] = {1,2,3,4};

void f(int *p){
	printf("%d\n",p[2]);
}

int main(void){
	f(x);

}

#endif


#if 0

#include <stdio.h> 

int x[4] = {1,2,3,4};

int *f1(void)
{
	return x;
}

void f2(int *p)
{
	printf("%d == %d == %d == %d\n", x[2], *(x+2), p[0], *p);
}

int main(void)
{
	int *p;
	int *a[4] = {x+3, x+2, x+1, x};

	p=x;

	printf("%d == %d\n", x[2], p[2]);
	printf("%d == %d == %d == %d\n", x[2], *(x+2), a[3][2], *a[1]);
	printf("%d == %d == %d == %d\n", x[2], *(x+2), f1()[2], *(f1()+2));
	f2(x+2);
	
	return 0;	
}

#endif

/***********************************************************/
// [7] : ����ü�� �⺻���̴�
/***********************************************************/

#if 0

#include <stdio.h>

int main(void) 
{
	int a, b = 10;

	a = b;
	printf("a=%d, b=%d\n", a, b);
	
	return 0;	
}

#endif

#if 0

#include <stdio.h>

int main(void) 
{
	struct st
	{
		int a;
		char b;
	} y = {100, 'A'};

	struct st x = y;

	printf("x.a=%d, x.b=%c\n", x.a, x.b);
	printf("y.a=%d, y.b=%c\n", y.a, y.b);
	
	return 0;	
}

#endif

/**********************************************************/
// [8] : ����ü�� �Լ� ����
/**********************************************************/

#if 0 

#include <stdio.h> 

struct math
{
	int id;
	char name[20];
	int score;
}mid, final={1, "Kim", 50};

int main(void)
{
	mid = final;
	
	printf("%d\n", mid.id);
	printf("%s\n", mid.name);
	printf("%d\n", mid.score);
	
	return 0;	
}

#endif

#if 0

#include <stdio.h>

struct math
{
	int id;
	char name[20];
	int score;
};

void cheat(struct math test);

int main(void)
{
	struct math final={1, "Kim", 50};
	cheat(final);
	printf("%d\n", final.score);
	
	return 0;	
}

void cheat(struct math test)
{
	test.score = 100;
}

#endif

/**********************************************************/
// [9] : ����ü �ּ��� �Լ� ����
/**********************************************************/

#if 0

#include <stdio.h> 
 
struct math
{
	int id;
	char name[20];
	int score;
};

void cheat(struct math * test);

int main(void)
{
	int a[] = {1,2,3,4};


	return 0;	
}


#endif
#if 0

#include <stdio.h> 
 
struct math
{
	int id;
	char name[20];
	int score;
};

void cheat(struct math * test);

int main(void)
{
	struct math final={1, "Kim", 50};
	cheat(&final);
	printf("%d\n", final.score);
	
	return 0;	
}

void cheat(struct math * test)
{
	(*test).score = 100;
}

#endif

/**********************************************************/
// [10] : 2�����迭�� ����
/**********************************************************/

#if 0

#include <stdio.h>

void draw_pixel(int y, int x, int value,  int (*p)[3] )
{
	p[y][x] = value;
	//p[y * 3 + x] = value;
}

int main(void)
{
	int a[2][3] = {{1,2,3},{4,5,6}};

	printf("%d\n", a[1][2]);
	draw_pixel(1, 2, 10, a);
	printf("%d\n", a[1][2]);
	
	return 0;	
}

#endif

/**********************************************************/
// [11] : 2�����迭�� ����
/**********************************************************/

#if 0

#include <stdio.h>


// 문자열의 정확한 타입은 const 다
void f(const char * p){
	printf("%c\n",p[1]);
}



int main(void)
{
	printf("%x\n", "hello" );
	printf("%c\n", "hello"[1] );
	printf("%c\n", *("hello"+1) );
	printf("%d\n", sizeof("hello") );

	f("hello");
	
	return 0;	
}

#endif

#if 0

#include <stdio.h>

typedef int(*FP1)[4];

 FP1 func(void)
{
	static int a[3][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
	return a;
}

int main(void)
{
	printf("%d\n",     func()[1][2]  );
	
	return 0;	
}

#endif

/**********************************************************/
// [12] : �Լ��� ����
/**********************************************************/

#if 0

#include <stdio.h> 

int add(int a, int b)
{
	return a+b;
}

int main(void)
{
	printf("%d\n", add(3,4));
	printf("%d\n", (&add)(3,4));
	printf("%d\n", (*add)(3,4));
	printf("%d\n", (**add)(3,4));
	
	return 0;	
}

#endif

/***********************************************************/
// [13] : 함수 등가포인터의 실행
/***********************************************************/

#if 0

#include <stdio.h>

int add(int a, int b)
{
	return a+b;
}	

void f1(void)
{
	printf("func\n");
}

int * f2(void)
{
	static int a[4] = {1,2,3,4};

	return a;
}

int main(void)
{
	// p, q, r ����
	int (*p)(int a, int b);
	void (* q)(void);
	int *(*r)(void);

	// p, q, r�� ���� �Լ� ����
	p = add;
	q = f1;
	r = f2;
	printf("%d\n", p(3,4));
	q();
	printf("%d\n", r()[2]);

	// ���� ������ ����� �������� p, q, r�� ����

	return 0;	
}

#endif

/***********************************************************/
// [14] : 함수에 함수를 ㄷ전달하다
/***********************************************************/

#if 0 

#include <stdio.h> 

int add(int a, int b)
{
	return a+b;
}

int sub(int a, int b)
{
	return a-b;
}

void func( int (*p)(int, int) )
{
	printf("%d\n", p(3,4));
}

int main(void)
{
	func(add);
	func(sub);
	
	return 0;	
}
#endif

/***********************************************************/
// [15] : load unbalancing solutions ---> 16
/***********************************************************/

#if 0

#include <stdio.h> 
#include <stdlib.h> 

int add(int a, int b)
{
	return a+b;
}

int sub(int a, int b)
{
	return a-b;
}

int mul(int a, int b)
{
	return a*b;
}

int get_key(void)
{
	return rand() % 3;
}

int op(int a, int b)
{
	switch(get_key())
	{
		case 0	: return add(a,b);
		case 1	: return sub(a,b);
		case 2	: return mul(a,b);
	}
}

int main(void)
{
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	
	return 0;	
}

#endif

/***********************************************************/
// [16] : function  Lookup table
/***********************************************************/

#if 1

#include <stdio.h> 
#include <stdlib.h> 

int add(int a, int b)
{
	return a+b;
}

int sub(int a, int b)
{
	return a-b;
}

int mul(int a, int b)
{
	return a*b;
}

int get_key(void)
{
	return rand() % 3;
}

	int (*fa[3])(int, int)  = {add, sub, mul};

int op(int a, int b)
{
	return fa[get_key()](a,b);
}

int main(void)
{
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	
	return 0;	
}

#endif
