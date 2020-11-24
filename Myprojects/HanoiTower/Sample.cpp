#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


//void HanoiTower(int n, char from, char temp, char to)
//{
//	if (n > 0)
//	{
//		HanoiTower(n - 1, from, to, temp);
//		printf("[%d]번 %c -> %c\n", n, from, to);
//		HanoiTower(n - 1, temp, from, to);
//	}
//
//
//}

void HanioTower(int n, char from, char temp, char to)
{
	if (n == 1)
	{
		printf("[%d]번 %c -> %c\n", n, from, to);
		
	}
	else
	{
		//a를 c을 임시저장소로 활용하여 b로 놓는다.
		HanioTower(n - 1, from, to, temp);
		printf("[%d]번 %c -> %c\n", n, from, to);
		
		//b를 a을 임시저장소로 활용하여 c로 놓는다.
		HanioTower(n - 1, temp, from, to);
	}
}

int Test(int n)
{

	if (n == 0)
	{
		return 0;
	}
	else
	{
		int sum = Test(n - 1);
		return n += sum;
	} 
}

int main()
{
	int n = 0;

	
	//printf("원반의 갯수를 입력하세요\n");

	//scanf("%d", &n);

	//int n = 3;


	//HanioTower(n,'A','B','c');

	printf("%d", Test(25));

}