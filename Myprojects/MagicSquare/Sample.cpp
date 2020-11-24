#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// ex) 5ĭ ������ ��� �� 25ĭ 1~25���� �� 325 / 5 = 65 5���� ���� ���� 65�� �Ǿ����. 

void MagicSquare(int** n, int x)
{
	int pin1 = 0;
	int pin2 = 0;

		// Ȧ�� ������
		int p = x;
		x = (x - 1);


		for (int i = 1; i < (p*p) + 1; i++)
		{
			if (i == 1)
			{
				n[x][x / 2] = i;
				pin1 = x;
				pin2 = x/2;
			}
			else if (pin1 + 1 > x && pin2 + 1 <= x)
			{
				n[0][pin2 + 1] = i;

				pin1 = 0;
				pin2 += 1;
			}
			else if (pin1 + 1 <= x && pin2 + 1 > x)
			{
				n[pin1 + 1][0] = i;

				pin1 +=1;
				pin2 = 0;
			}
			else if (pin1 + 1 > x && pin2 + 1 > x)
			{
				n[pin1 - 1][pin2] = i;

				pin1 -= 1;
			}
			else if (n[pin1 + 1][pin2 + 1] == 0)
			{
				n[pin1 + 1][pin2 + 1] = i;

				pin1 += 1;
				pin2 += 1;
			}
			else if (pin1 +1 <=x || pin2 + 1 <= x)
			{
				if (n[pin1 + 1][pin2 + 1] == 0)
				{
					n[pin1 + 1][pin2 + 1] = i;
					pin1 += 1;
					pin2 += 1;
				}
				else
				{
					n[pin1 - 1][pin2] = i;
					pin1 -= 1;
				}
			}
		else
		{
				printf("�ù�");
		}

	}
		for (int i = 0; i < p; i++)
		{
			for (int j = 0; j < p; j++)
			{
				printf("%4d", n[i][j]);
			}
			printf("\n");
		}

}

int main()
{
	
	int x = 0;
	printf("�������� ������ �Է��ϼ���");
	scanf("%d", &x);

	int** n;
	n = (int**)malloc(sizeof(intptr_t) * x);
	for (int i = 0; i < x; i++)
	{
		n[i] = (int *)malloc(sizeof(int)*x);
	}

	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < x; j++)
		{
			n[i][j] = 0;
		}
	}


	MagicSquare(n, x);

	for (int i = 0; i < x; i++)
	{
		free(n[i]);
	}
	free(n);
}