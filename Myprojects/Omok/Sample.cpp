#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>

#define   RIGHT   77
#define LEFT   75
#define UP      72
#define DOWN   80
#define ENTER 13

int g_OmokBoard[19][19];
int x = 10;
int y = 10;
int bb = 0;


enum gameM
{
	gameS = 0,
	gmaeMi,
	gmaeE
};

void GameBoardC()
{
	system("cls");
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					if (g_OmokBoard[j][i] == 1)          printf("○");
					else if (g_OmokBoard[j][i] == 2)      printf("●");
					else if (g_OmokBoard[j][i] == 0)      printf("┌");
					else if (g_OmokBoard[j][i] == 3)      printf("ㅁ");
				}
				else if (j == 18)
				{
					if (g_OmokBoard[j][i] == 1)         printf("○");
					else if (g_OmokBoard[j][i] == 2)      printf("●");
					else if (g_OmokBoard[j][i] == 0)      printf("┐");
					else if (g_OmokBoard[j][i] == 3)      printf("ㅁ");
				}
				else
				{
					if (g_OmokBoard[j][i] == 1)         printf("○");
					else if (g_OmokBoard[j][i] == 2)      printf("●");
					else if (g_OmokBoard[j][i] == 0)      printf("┬");
					else if (g_OmokBoard[j][i] == 3)      printf("ㅁ");
				}
			}
			else if (i == 18)
			{
				if (j == 0)
				{
					if (g_OmokBoard[j][i] == 1)         printf("○");
					else if (g_OmokBoard[j][i] == 2)      printf("●");
					else if (g_OmokBoard[j][i] == 0)      printf("└");
					else if (g_OmokBoard[j][i] == 3)      printf("ㅁ");
				}
				else if (j == 18)
				{
					if (g_OmokBoard[j][i] == 1)         printf("○");
					else if (g_OmokBoard[j][i] == 2)      printf("●");
					else if (g_OmokBoard[j][i] == 0)      printf("┘");
					else if (g_OmokBoard[j][i] == 3)      printf("ㅁ");
				}
				else
				{
					if (g_OmokBoard[j][i] == 1)         printf("○");
					else if (g_OmokBoard[j][i] == 2)      printf("●");
					else if (g_OmokBoard[j][i] == 0)      printf("┴");
					else if (g_OmokBoard[j][i] == 3)      printf("ㅁ");
				}
			}
			else
			{
				if (j == 0)
				{
					if (g_OmokBoard[j][i] == 1)         printf("○");
					else if (g_OmokBoard[j][i] == 2)      printf("●");
					else if (g_OmokBoard[j][i] == 0)      printf("├");
					else if (g_OmokBoard[j][i] == 3)      printf("ㅁ");
				}
				else if (j == 18)
				{
					if (g_OmokBoard[j][i] == 1)         printf("○");
					else if (g_OmokBoard[j][i] == 2)      printf("●");
					else if (g_OmokBoard[j][i] == 0)      printf("┤");
					else if (g_OmokBoard[j][i] == 3)      printf("ㅁ");
				}
				else
				{
					if (g_OmokBoard[j][i] == 1)         printf("○");
					else if (g_OmokBoard[j][i] == 2)      printf("●");
					else if (g_OmokBoard[j][i] == 0)      printf("┼");
					else if (g_OmokBoard[j][i] == 3)      printf("ㅁ");
				}
			}
		}
		printf("\n");
	}
}

int WL()
{
	int iCunt = 0;
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (g_OmokBoard[i][j] == 1 || g_OmokBoard[i][j] == 2)
			{

				for (int jCunt = 0; jCunt < 5; jCunt++)
				{
					if (g_OmokBoard[i][j] == g_OmokBoard[i + jCunt][j])
					{
						iCunt++;
					}
				}
				if (iCunt == 5)
				{
					return g_OmokBoard[i][j];
				}
				iCunt = 0;

				for (int jCunt = 0; jCunt < 5; jCunt++)
				{
					if (g_OmokBoard[i][j] == g_OmokBoard[i - jCunt][j])
					{
						iCunt++;
					}
				}
				if (iCunt == 5)
				{
					return g_OmokBoard[i][j];
				}
				iCunt = 0;

				for (int jCunt = 0; jCunt < 5; jCunt++)
				{
					if (g_OmokBoard[i][j] == g_OmokBoard[i][j + jCunt])
					{
						iCunt++;
					}
				}
				if (iCunt == 5)
				{
					return g_OmokBoard[i][j];
				}
				iCunt = 0;

				for (int jCunt = 0; jCunt < 5; jCunt++)
				{
					if (g_OmokBoard[i][j] == g_OmokBoard[i][j - jCunt])
					{
						iCunt++;
					};
				}
				if (iCunt == 5)
				{
					return g_OmokBoard[i][j];
				}
				iCunt = 0;

				for (int jCunt = 0; jCunt < 5; jCunt++)
				{
					if (g_OmokBoard[i][j] == g_OmokBoard[i + jCunt][j + jCunt])
					{
						iCunt++;
					};
				}
				if (iCunt == 5)
				{
					return g_OmokBoard[i][j];
				}
				iCunt = 0;

				for (int jCunt = 0; jCunt < 5; jCunt++)
				{
					if (g_OmokBoard[i][j] == g_OmokBoard[i - jCunt][j + jCunt])
					{
						iCunt++;
					};
				}
				if (iCunt == 5)
				{
					return g_OmokBoard[i][j];
				}
				iCunt = 0;

				for (int jCunt = 0; jCunt < 5; jCunt++)
				{
					if (g_OmokBoard[i][j] == g_OmokBoard[i + jCunt][j - jCunt])
					{
						iCunt++;
					};
				}
				if (iCunt == 5)
				{
					return g_OmokBoard[i][j];
				}
				iCunt = 0;

				for (int jCunt = 0; jCunt < 5; jCunt++)
				{
					if (g_OmokBoard[i][j] == g_OmokBoard[i - jCunt][j - jCunt])
					{
						iCunt++;
					};
				}
				if (iCunt == 5)
				{
					return g_OmokBoard[i][j];
				}
				iCunt = 0;

			}
		}
	}
}

int xyc(int xp, int yp, int wb)
{
	bb = g_OmokBoard[xp][yp];

	g_OmokBoard[xp][yp] = 3;


	bool Enter = false;
	while (Enter != true)
	{

		switch (_getch())
		{
		case RIGHT:
		{
			if (x == 18)
				break;

			g_OmokBoard[xp][yp] = bb;
			bb = g_OmokBoard[xp + 1][yp];
			g_OmokBoard[xp + 1][yp] = 3;
			xp++;
			x++;
		}break;
		case LEFT:
		{
			if (x == 0)
				break;
			g_OmokBoard[xp][yp] = bb;
			bb = g_OmokBoard[xp - 1][yp];
			g_OmokBoard[xp - 1][yp] = 3;
			xp--;
			x--;
		}break;

		case UP:
		{
			if (y == 0)
				break;
			g_OmokBoard[xp][yp] = bb;
			bb = g_OmokBoard[xp][yp - 1];
			g_OmokBoard[xp][yp - 1] = 3;
			yp--;
			y--;
		}break;

		case DOWN:
		{
			if (y == 18)
				break;
			g_OmokBoard[xp][yp] = bb;
			bb = g_OmokBoard[xp][yp + 1];
			g_OmokBoard[xp][yp + 1] = 3;
			yp++;
			y++;
		}break;
		case ENTER:
		{
			if (bb != 1 && bb != 2)
			{
				if (wb == 1)
				{
					g_OmokBoard[xp][yp] = 1;
				}
				else
				{
					g_OmokBoard[xp][yp] = 2;
				}
				Enter = true;
			}
		}break;
		}
		GameBoardC();
	}
	return 0;
}

void g_OnokBcls()
{
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			g_OmokBoard[i][j] = 0;
		}
	}
}

int main(void)
{
	char KeyB;
	int Win = 0;
	int gmae = gameS;

	int wb = 1; // 1 백 먼저 시작 , 2 흑 먼저 시작
	//1 화이트 승리 2 블랙 승리
	while (Win != 1 && Win != 2)
	{
		switch (gmae)
		{
		case gameS:
		{
			g_OnokBcls();
			GameBoardC();
			gmae = gmaeMi;
		}break;

		case gmaeMi:
		{

			xyc(x, y, wb);

			//GameBoardC();

			if (wb == 1)
			{
				wb++;
			}
			else
			{
				wb--;
			}
			gmae = gmaeE;
		}break;
		case gmaeE:
		{
			Win = WL();

			gmae = gmaeMi;
			// 게임 끝
		}break;
		}
	}

	system("cls");

	if (Win == 1)
	{
		printf("백색이 이겻음");
	}
	else
	{
		printf("검정색이 이겼음");
	}

}