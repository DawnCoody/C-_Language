#include <WindowS.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#define MAP_WIDTH 80 //地图宽度
#define MAP_HEGHT 20 //地图高度
// *(创建地图、食物) @(蛇头) *(蛇身)

void Map_Frame();          //地图框架
void SetPos(int x, int y); //控制光标位置
void DrawSnake();          //画蛇
void Move();        //移动
void HideCursor();		//隐藏光标
void DrawFood();	//画食物

struct object
{
	int x;   //x坐标
	int y;   //y坐标
	char ch; //@(蛇头) 或 *(蛇身) 或 #（食物）
};

//结构体数组，代表蛇
struct object g_snack[MAP_WIDTH * MAP_HEGHT];
//蛇的实际的长度
int g_snack_length = 5;
//食物
struct object g_food;
int g_score = 0;

int main(void)
{
	srand((unsigned	int)time(NULL));	//初始化随机数种子
	HideCursor();	//隐藏光标

	Map_Frame(); //绘制地图

	//初始化蛇的长度
	g_snack_length = 5;
	//初始化蛇
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

	DrawSnake(); //画蛇
	Move(); //使蛇移动

	system("cls");
	printf("游戏结束得分%d", g_score);

	return 0;
}

void Map_Frame() ////绘制地图框架
{
	system("color f4");
	printf("\n\t\t\t\t\t欢迎来到我的贪吃蛇\n\t\t\t\t\t   用AWDS控制方向");
	Sleep(3000);
	system("cls");
	system("color b1");
	for (int j = 0; j <= MAP_HEGHT; j++)
	{
		if (j == 0 || j == MAP_HEGHT)
		{
			//绘制上下边框
			for (int i = 0; i < MAP_WIDTH; i += 1)
			{
				printf("*");
			}
		}
		else //绘制右边框
		{
			printf("*");
			//绘制空各
			for (int i = 0; i < MAP_WIDTH - 2; i++)
			{
				printf(" ");
			}
			printf("*");
		}
		printf("\n");
	}
}

void SetPos(int x, int y) //控制光标位置
{
	COORD point = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, point);
}

void DrawSnake() //画蛇
{
	for (int i = 0; i < g_snack_length; i++)
	{
		//设置光标到（0，0）的位置
		SetPos(g_snack[i].x, g_snack[i].y);

		//绘制贪吃蛇 ****@
		printf("%c", g_snack[i].ch);
	}
}

void Move()
{
	g_food.x = rand() % (MAP_WIDTH - 2) + 2;
	g_food.y = rand() % (MAP_HEGHT -2) + 2;
	g_food.ch = '#';
	DrawFood();
	int key = 'd';//默认向右移动
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

		//清除尾坐标
		SetPos(tail_x, tail_y);
		putchar(' ');
		DrawSnake();

		//贪吃蛇撞墙
		if (g_snack[g_snack_length - 1].x == MAP_WIDTH -1 || g_snack[g_snack_length - 1].x == 0
			|| g_snack[g_snack_length - 1].y == MAP_HEGHT || g_snack[g_snack_length - 1].y == 0)
		{
			break;
		}

		//吃食物变长
		if (g_snack[g_snack_length - 1].x == g_food.x && g_snack[g_snack_length - 1].y == g_food.y)
		{
			g_snack_length++;

			//移动数组元素
			for (int i = g_snack_length - 1; i > 0; i--)
			{
				g_snack[i].x = g_snack[i - 1].x;
				g_snack[i].y = g_snack[i - 1].y;
				g_snack[i].ch = g_snack[i - 1].ch;
			}
			//尾坐标
			g_snack[0].x = tail_x;
			g_snack[0].y = tail_y;
			g_snack[0].ch = '*';

			g_food.x = rand() % (MAP_WIDTH -4) + 2;
			g_food.y = rand() % (MAP_HEGHT -4) + 2;

			//生成新的食物
			DrawFood();
			//吃到食物得10分
			g_score += 10;
		}
		//咬到自己结束
		if (EatSelf() < 0)
		{
			break;
		}

	}
}

void HideCursor()	//隐藏光标
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = 0; //隐藏控制台光标
	SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
}

void DrawFood()
{
	//设置光标的位置
	SetPos(g_food.x, g_food.y);

	//绘制#(食物)
	printf("%c", g_food.ch);

}

//自己吃到自己凉凉
int EatSelf()
{
	for (int i = 0; i < g_snack_length - 1; i++)
	{
		if (g_snack[g_snack_length - 1].x == g_snack[i].x && g_snack[g_snack_length - 1].y == g_snack[i].y)
		{
			//咬到自己
			return -1;
		}
	}
	return 0;
}