#include <stdio.h>
//#include<math.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 256
#define MAX 200
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio_ext.h>
#include <ctype.h>
//Do không sử dụng thư viện math.h nên làm hàm pow1
int pow1(int a, int n);
int pow1(int a, int n)
{
	if (n == 1)
	{
		return a;
	}
	else
	{
		return pow1(a, n - 1)*a;
	}
}
//------------------------------------
//struct stack và các hàm liên quan đến stack
typedef struct stack
{
	float data[MAX];
	int top;
}stack;

char peek(stack *);
int priority(char);
void init(stack *);
int empty(stack *);
int full(stack *);
float pop(stack *);
void push(stack *, float);
char top(stack *);
//-------------------------------------------------
//thay thế một ký tự (hoặc chuỗi ) bằng một ký tự hoặc chuỗi khác

void _str_rpl(char *s, char *s1, char *s2)
{
	int ns = strlen(s), ns1 = strlen(s1), ns2 = strlen(s2);
	int i = 0, j, pst;
	while (s[i])
	{
		pst = i;
		j = 0;
		while (s1[j] && s[pst] == s1[j]) pst++, j++;
		if (j == ns1)
		{
			memmove(s + i + ns2, s + i + ns1, ns - i - ns1 + 1);
			memcpy(s + i, s2, ns2);
			ns += ns2 - ns1;
			i += ns2;
		}
		else i++;
	}
	//printf("%s", s);
}
#pragma region Phần hàm tính đảm nhân công việc tính toán
//tính toàn dựa trên Infix to Postfix and  evaluate postfix expression using stack 
float Trans(char infix[])
{
	char item;

	int m = 0;
	int count = 0;
	float chua = 0;
	//khởi tạo stack rỗng tên _stack
	stack _stack;
	init(&_stack);
	//---------------------------
	int i = 0;
	char potfix[MAX];
	int cham = 0;
	char item_before;

	char _zero[MAX] = "0";
	//push(&_stack, "asdf");
	while (infix[i] != '\0')
	{
		item = infix[i];
		//kiểm tra xem item có phải là số không
		if (isdigit(infix[i]))
		{
			//-----------------------------------
			potfix[m++] = infix[i];
		}
		//giải quyết vấn đề khi nhập vào số thập phân
		else if (item == ' ' || item == '.')
		{
			potfix[m++] = item;
		}
		//---------------------------------------------
		else if (item == '(')
		{
			push(&_stack, '(');
		}
		else if (item == ')')
		{
			while (top(&_stack) != '(')
			{
				potfix[m++] = pop(&_stack);
			}
			pop(&_stack);
		}

		else if (priority(item) > 0)
		{
			//sử dụng cho trường hợp có kiểu (-a) hay(+a) trong đó a là một số
			//khi đó thì sẽ chèn vào phía trước nó một số không với dạng (0-a) hay (0+a)
			if (!(isdigit(item)) && (item_before == '('))
			{
				while (_zero[count] != '\0')
				{
					potfix[m++] = _zero[count];
					count++;
				}
			}
			//-----------------------------------------------------------------
			while ((!empty(&_stack)) && (priority(item) <= priority(top(&_stack))) && (priority(top(&_stack)) != -1) && priority(top(&_stack)) != 0)
			{

				potfix[m++] = pop(&_stack);

			}

			push(&_stack, item);


		}
		if (item == ' ' || isblank(item))//item == NULL)
		{

		}
		else
		{
			//để lưu giá trị item dùng kiểm tra cho quá trình xác định vấn đề dấu ngoặc '('
			item_before = item;
		}

		i++;
	}
	while (!empty(&_stack))
	{
		potfix[m++] = pop(&_stack);

	}
	//--------------------------

	i = 0;
	item = ' ';
	init(&_stack);
	float number = 0;
	float mu = 0;
	while (potfix[i] != '\0')
	{
		item = potfix[i];
		if (potfix[i] == ' ')
		{

		}

		else if (isdigit(item))
		{

			//giải quyết các vấn đề khi nhập vào là hai chữ số trở lên hay chữ số thập phân 
			while (isdigit(potfix[i]) || potfix[i] == '.')
			{
				item = potfix[i];
				//nếu không trừ 48 thì nó lưu vào là mã ASCII của số chứ không phải là số
				//biến chấm dùng để báo hiệu khi có xuất hiện số thập phân
				//biến mũ thì để báo hiệu khi có sự xuất hiện của số có hai chữ số trở lên

				if (mu > 0 && potfix[i] != '.')
				{
					number = number * 10 + (float)potfix[i] - 48;
				}
				else if (potfix[i] != '.')
				{
					number = potfix[i] - 48;
				}
				else if (cham > 0 && (isdigit(potfix[i + 1])))
				{
					cham++;
				}
				else if (cham > 0 && (!isdigit(potfix[i + 1])))
				{
					number = number / (pow1(10, cham));
				}
				else if (potfix[i] == '.')
				{
					cham++;
				}

				i++;
				mu++;
			}
			if (cham > 0)
			{
				number = number / (pow1(10, cham));
			}
			//--------------------------------------
			push(&_stack, (float)number);
			number = 0;
			cham = 0;
			mu = 0;
			continue;
			//---------------------------------------
		}
		else if (item == '+' || item == '*' || item == '-' || item == '/')
		{
			float _number2 = pop(&_stack);
			float _number1 = pop(&_stack);
			if (item == '+')
			{
				chua = _number1 + _number2;
				push(&_stack, chua);


			}
			else if (item == '-')
			{
				chua = _number1 - _number2;
				push(&_stack, _number1 - _number2);
			}
			else if (item == '*')
			{
				chua = _number1 * _number2;
				push(&_stack, chua);
			}
			else if (item == '/')
			{
				chua = _number1 / _number2;
				push(&_stack, chua);

			}

		}
		i++;


	}
	float _result = pop(&_stack);
	return _result;
	//_de_bug = 0;
	//printf("%f\n", _result);
	//printf("%s", pop(&_stack));
}
//-------------------------------------------
#pragma endregion

#pragma region Đây là phần hàm cho Stack
int priority(char x)
{
	if (x == '(' || x == ')')
		return(-1);
	if (x == '+' || x == '-')
		return(1);
	if (x == '*' || x == '/' || x == '%')
		return(2);
	return(0);
}
//---------------------------------------------
void init(stack *s)
{
	s->top = -1;
}
//---------------------------------------------
int empty(stack *s)
{
	if (s->top == -1)
		return(1);
	else
		return(0);
}
//---------------------------------------------
int full(stack *s)
{
	if (s->top == MAX - 1)
		return(1);
	else
		return(0);
}
//---------------------------------------------
void push(stack *s, float x)
{
	s->top = s->top + 1;
	s->data[s->top] = x;
}
//---------------------------------------------
char peek(stack *s)
{
	int x;
	x = s->data[s->top];
	s->top = s->top;
	return(x);
}
//---------------------------------------------
float pop(stack *s)
{
	float x;
	x = (float)(s->data[s->top]);
	s->top = s->top - 1;
	return((float)x);
}
//---------------------------------------------
char top(stack * s)
{
	return(s->data[s->top]);
}
//---------------------------------------------
#pragma endregion

#pragma region cấu trúc của phần thông điệp của MessageQueue và SharedMemoory
//phần thông điệp của ShareMemory
typedef struct {
	char test1[MAX];
	float test2;
}sm;
//---------------------------------
//Phần cấu trúc thông điệp cùa Message Queue
struct Message
{
	long mtype;
	char content[MAX];
};
//--------------------------------------------

#pragma endregion

#pragma region Phần chương trình chính
//sử dụng Message Queue
int main_for_Message_Queue(int argc, char* argv[])
{
	//char * string;
	int mid;
	//int id;
	//int child;
	key_t key;
	struct  Message msg;
	int n;
	//msg.content=(char*)malloc(BUFSIZ);
	if (key = ftok(".", 'a') == -1)
	{
		perror("key created\n");
		return 1;
	}
	if ((mid = msgget(key, IPC_CREAT | 0666)) == -1)
	{
		perror("Queue created\n");
		return 2;
	}
	switch (fork())
	{
		//0 la chid
		//khac 0 la parent
	case -1: perror("fork error\n");
		return 3;
	case 0:
		sleep(15);
		//int debug1 = 0;
		//int debug2 = 0;
		if ((n = msgrcv(mid, &msg, sizeof(msg), 10, 0)) == -1)
		{
			perror("Receive\n");
			return 5;
		}
		printf("chuoi nhap %s\n", msg.content);
		
		_str_rpl(msg.content, "+", " + ");
		_str_rpl(msg.content, "-", " - ");
		_str_rpl(msg.content, "*", " * ");
		_str_rpl(msg.content, "/", " / ");
		_str_rpl(msg.content, "(", " ( ");
		_str_rpl(msg.content, ")", " ) ");
		
		float kq = Trans(msg.content);
		sprintf(msg.content, "%f", kq);

		//printf("Parent receive from chid:\n");

		printf("sau tinh toan %s\n", msg.content);
		//printf("------------------\n");
		//printf("Message from parent\n");

		msg.mtype = 11;

		if ((msgsnd(mid, &msg, sizeof(msg), 0)) == -1)
		{
			perror("Message Send");
			return 4;
		}
		return 10;
	default:
		printf("nhap Message Queue\n");
		__fpurge(stdin);
		gets(msg.content);


		//msg.content=&string;

		msg.mtype = 10;
		if (msgsnd(mid, &msg, sizeof(msg), 0) == -1)
		{
			perror("Send loi\n");
			return 4;
		}
		wait(NULL);
		printf("ket qua :\n");
		if ((n = msgrcv(mid, &msg, sizeof(msg), 11, 0)) == -1)
		{
			perror("Receive\n");
			return 5;
		}

		printf("%s\n", msg.content);
		return 0;
	}
	return 0;
}
//sử dụng SharedMemory
int main(int argc, char* argv[])
{
	sm * shm;
	int  shmid;
	key_t key;
	if ((key = ftok(".", 'a')) == -1)
	{
		perror("Created");
		return  1;

	}
	if ((shmid = shmget(key, SIZE, IPC_CREAT | 0666)) == -1)
	{
		perror("SM");
		return 2;
	}


	shm = (sm *)shmat(shmid, 0, 0);
	switch (fork())
	{
	case -1:perror("error");
		return 4;
	case 0:
		sleep(15);

		shm->test2 = Trans(shm->test1);
		shmdt((void*)shm);
		_exit(1);

	default:
		printf("nhap Shared Memory :\n"); __fpurge(stdin);

		gets(shm->test1);

		_str_rpl(shm->test1, "+", " + ");
		_str_rpl(shm->test1, "-", " - ");
		_str_rpl(shm->test1, "*", " * ");
		_str_rpl(shm->test1, "/", " / ");
		_str_rpl(shm->test1, "(", " ( ");
		_str_rpl(shm->test1, ")", " ) ");

		wait(NULL);

		printf("la %12.3f\n", shm->test2);

		shmdt((void*)shm);

		shmctl(shmid, IPC_RMID, (struct shmid_ds*)0);

	}
	return 0;
}

#pragma endregion

