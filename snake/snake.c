#include <WindowS.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#define MAP_WIDTH 80 //��ͼ���
#define MAP_HEGHT 20 //��ͼ�߶�
// *(������ͼ��ʳ��) @(��ͷ) *(����)

void Map_Frame();          //��ͼ���
void SetPos(int x, int y); //���ƹ��λ��
void DrawSnake();          //����
void Move();        //�ƶ�
void HideCursor();		//���ع��
void DrawFood();	//��ʳ��

struct object
{
	int x;   //x����
	int y;   //y����
	char ch; //@(��ͷ) �� *(����) �� #��ʳ�
};

//�ṹ�����飬������
struct object g_snack[MAP_WIDTH * MAP_HEGHT];
//�ߵ�ʵ�ʵĳ���
int g_snack_length = 5;
//ʳ��
struct object g_food;
int g_score = 0;

int main(void)
{
	srand((unsigned	int)time(NULL));	//��ʼ�����������
	HideCursor();	//���ع��

	Map_Frame(); //���Ƶ�ͼ

	//��ʼ���ߵĳ���
	g_snack_length = 5;
	//��ʼ����
	g_snack[0].x = 4;
	g_snack[0].y = 3;
	g_snack[0].ch = '*';

	g_snack[1].x = 5;
	g_snack[1].y = 3;
	g_snack[1].ch = '*';

	g_snack[2].x = 6;
	g_snack[2].y = 3;
	g_snack[2].ch = '*';

	g_snack[3].x = 7;
	g_snack[3].y = 3;
	g_snack[3].ch = '*';

	g_snack[4].x = 8;
	g_snack[4].y = 3;
	g_snack[4].ch = '@';

	DrawSnake(); //����
	Move(); //ʹ���ƶ�

	system("cls");
	printf("��Ϸ�����÷�%d", g_score);

	return 0;
}

void Map_Frame() ////���Ƶ�ͼ���
{
	system("color f4");
	printf("\n\t\t\t\t\t��ӭ�����ҵ�̰����\n\t\t\t\t\t   ��AWDS���Ʒ���");
	Sleep(3000);
	system("cls");
	system("color b1");
	for (int j = 0; j <= MAP_HEGHT; j++)
	{
		if (j == 0 || j == MAP_HEGHT)
		{
			//�������±߿�
			for (int i = 0; i < MAP_WIDTH; i += 1)
			{
				printf("*");
			}
		}
		else //�����ұ߿�
		{
			printf("*");
			//���ƿո�
			for (int i = 0; i < MAP_WIDTH - 2; i++)
			{
				printf(" ");
			}
			printf("*");
		}
		printf("\n");
	}
}

void SetPos(int x, int y) //���ƹ��λ��
{
	COORD point = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, point);
}

void DrawSnake() //����
{
	for (int i = 0; i < g_snack_length; i++)
	{
		//���ù�굽��0��0����λ��
		SetPos(g_snack[i].x, g_snack[i].y);

		//����̰���� ****@
		printf("%c", g_snack[i].ch);
	}
}

void Move()
{
	g_food.x = rand() % (MAP_WIDTH - 2) + 2;
	g_food.y = rand() % (MAP_HEGHT -2) + 2;
	g_food.ch = '#';
	DrawFood();
	int key = 'd';//Ĭ�������ƶ�
	while (1)
	{
		int tail_x = g_snack[0].x;
		int tail_y = g_snack[0].y;

		for (int i = 0; i < g_snack_length - 1; i++)
		{
			g_snack[i].x = g_snack[i + 1].x;
			g_snack[i].y = g_snack[i + 1].y;
		}
		Sleep(100);
		if (_kbhit() != 0)
		{
			key = _getch();
		}
		switch (key)
		{
		case 'w':
		case (char)38:
			g_snack[g_snack_length - 1].y -= 1;
			break;
		case's':
		case (char)40:
			g_snack[g_snack_length - 1].y += 1;
			break;
		case 'a':
		case (char)37:
			g_snack[g_snack_length - 1].x -= 1;
			break;
		case 'd':
		case (char)39:
			g_snack[g_snack_length - 1].x += 1;
			break;
		}

		//���β����
		SetPos(tail_x, tail_y);
		putchar(' ');
		DrawSnake();

		//̰����ײǽ
		if (g_snack[g_snack_length - 1].x == MAP_WIDTH -1 || g_snack[g_snack_length - 1].x == 0
			|| g_snack[g_snack_length - 1].y == MAP_HEGHT || g_snack[g_snack_length - 1].y == 0)
		{
			break;
		}

		//��ʳ��䳤
		if (g_snack[g_snack_length - 1].x == g_food.x && g_snack[g_snack_length - 1].y == g_food.y)
		{
			g_snack_length++;

			//�ƶ�����Ԫ��
			for (int i = g_snack_length - 1; i > 0; i--)
			{
				g_snack[i].x = g_snack[i - 1].x;
				g_snack[i].y = g_snack[i - 1].y;
				g_snack[i].ch = g_snack[i - 1].ch;
			}
			//β����
			g_snack[0].x = tail_x;
			g_snack[0].y = tail_y;
			g_snack[0].ch = '*';

			g_food.x = rand() % (MAP_WIDTH -4) + 2;
			g_food.y = rand() % (MAP_HEGHT -4) + 2;

			//�����µ�ʳ��
			DrawFood();
			//�Ե�ʳ���10��
			g_score += 10;
		}
		//ҧ���Լ�����
		if (EatSelf() < 0)
		{
			break;
		}

	}
}

void HideCursor()	//���ع��
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
	CursorInfo.bVisible = 0; //���ؿ���̨���
	SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
}

void DrawFood()
{
	//���ù���λ��
	SetPos(g_food.x, g_food.y);

	//����#(ʳ��)
	printf("%c", g_food.ch);

}

//�Լ��Ե��Լ�����
int EatSelf()
{
	for (int i = 0; i < g_snack_length - 1; i++)
	{
		if (g_snack[g_snack_length - 1].x == g_snack[i].x && g_snack[g_snack_length - 1].y == g_snack[i].y)
		{
			//ҧ���Լ�
			return -1;
		}
	}
	return 0;
}