#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void Diamond()
{
	int Vertical;
	printf("\n ========== º° °¹¼ö ==========");
	scanf("%d", &Vertical);
	for (int i = 0; i < Vertical; i++)
	{
		for (int j = 0; j < (Vertical - i - 1); j++)
		{
			printf("-");
		}
		for (int k = 0; k < i*2+1; k++)
		{
			printf("*");
		}

		for (int j = 0; j < (Vertical - i - 1); j++)
		{
			printf("-");
		}
		printf("\n");
	}
	for (int i = (Vertical - 1); i > 0; i--)
	{
		for (int j = 0; j < (Vertical - i); j++)
		{
			printf("-");
		}
		for (int k = 0; k < i * 2 - 1; k++)
		{
			printf("*");
		}
		for (int j = 0; j < (Vertical - i); j++)
		{
			printf("-");
		}
		printf("\n");
	}
}

int main()
{

	Diamond();

}