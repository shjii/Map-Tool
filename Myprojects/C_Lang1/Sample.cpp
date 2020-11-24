#define _CRT_SECURE_NO_WARNINGS
#define CPLUSPLUS
#undef CPLUSPLUS

#ifndef CPLUSPLUS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
void main()
{
	int intValue = 5;
	int* pValue = &intValue;
	int** ppValue = &pValue;
	printf("%d", intValue);
	printf("%d", *pValue);
	printf("%d", **ppValue);

	printf("\n================\n");
	int *pPont = (int*)malloc(sizeof(int));
	*pPont = 100;

#ifdef _WIN64
	printf("%016llx���� : %d\n", pPont + 0, *(pPont)+0);
#else
	printf("%016x���� : %d\n", pPont + 0, *(pPont)+0);
#endif
	free(pPont);

	printf("\n====================\n");
	int *pPointArray = (int*)malloc(sizeof(int) * 10);
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		pPointArray[iCnt] = iCnt * 10;
		printf("\n0x%08x���� : %d", &pPointArray[iCnt], *(pPointArray + iCnt));
	}
	free(pPointArray);

	printf("\n =======���ڸ� �Է��ϰ� ���͸��ÿ� !===========\n");
	char*  pCharPoint = 0;
	int iCount = 0;
	pCharPoint = (char*)malloc(sizeof(char) * 100);
	do
	{
		int ivalue = _getch();
		if (ivalue != 0 && ivalue != 0x00E0)
		{
			pCharPoint[iCount++] = ivalue;
			printf("*");
		}
	} while (pCharPoint[iCount - 1] != '\r');

	pCharPoint[iCount - 1] = 0;

	printf("\n0x%08x���� : %s", pCharPoint, pCharPoint);

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		printf("\n0x%08X���� : %c", pCharPoint + iCnt, *(pCharPoint + iCnt));
	}
	free(pCharPoint);
	printf("\n=====================\n");
	int *lpPoint[10];
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		lpPoint[iCnt] = (int*)malloc(sizeof(int));
		*lpPoint[iCnt] = iCnt * 10;
		printf("\n0x%08X���� : %d", lpPoint + iCnt, *lpPoint[iCnt]);
	}
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		free(lpPoint[iCnt]);
	}
	printf("\n=============\n");

	////////////////////////////////////////////////////////////////

	int iValue[3][2] = { {10,20},{30,40},{50,60} };
	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		printf("\n*(iValue+%d): %p", iCnt, *(iValue + iCnt));
		printf("\t\tiValue[%d]: %p", iCnt, iValue[iCnt]);
	}
	printf("\n");

	int* pData = iValue[1];
	*pData++ = *pData + 100;
	*pData++ = *pData + 100;
	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		printf("\n*(iValue+%d): %p", iCnt, *(iValue + iCnt));
		printf("\t\tiValue[%d]: %p", iCnt, iValue[iCnt]);
	}
	printf("\n");

	pData = iValue[0];
	int** ppData = &pData;
	ppData[0][0] = *(*(ppData + 0) + 0) + 200;
	ppData[0][1] = *(*(ppData + 0) + 1) + 200;

	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		printf("\n*(iValue+%d): %p", iCnt, *(iValue + iCnt));
		printf("\t\tiValue[%d]: %p", iCnt, iValue[iCnt]);
	}
	printf("\n");

	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		for (int jCnt = 0; jCnt < 2; jCnt++)
		{
			printf("\n*(*(iValue+%d)+%d)  : %d", iCnt, jCnt, *(*(iValue + iCnt) + jCnt));
			printf("\t\t *(iValue[%d]+%d) : %d", iCnt, jCnt, *(iValue[iCnt] + jCnt));
		}
	}
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
	//////////////////////

	printf("\n===============\n");
	system("cls");// ȭ�� Ŭ����
	int *pPtr;
	int **ppPtr;
	int num = 10;
	pPtr = &num;
	ppPtr = &pPtr;
	printf("%d", **ppPtr);
	unsigned char *array[50];
	unsigned int size = sizeof(array);

#ifdef _WIN64		
	printf("\n%d", size);
	printf("\n64 - bit Size=%d", sizeof(array));
#else
	printf("\n32 - bit Size=%d", sizeof(array));
#endif	

	int **ppPoint = 0;
	ppPoint = (int **)malloc(sizeof(intptr_t) * 3);
	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		ppPoint[iCnt] = (int *)malloc(sizeof(int) * 3);
	}

	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		for (int jCnt = 0; jCnt < 3; jCnt++)
		{
			ppPoint[iCnt][jCnt] = iCnt * 3 + jCnt;
		}
	}

	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		printf("\n");
		for (int jCnt = 0; jCnt < 3; jCnt++)
		{
			printf("[%d, %d]=%d: %d", iCnt, jCnt, ppPoint[iCnt][jCnt], *(ppPoint[iCnt] + jCnt));
		}
	}

	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		free(ppPoint[iCnt]);
	}
	free(ppPoint);


	//////

	printf("\n ====== \n");
	system("cls");
	char *pArray[6];
	for (int iCnt = 0; iCnt < 6; iCnt++)
	{
		pArray[iCnt] = (char*)malloc(sizeof(char) * 4);
	}

	for (int iCnt = 0; iCnt < 6; iCnt++)
	{
		for (int jCnt = 0; jCnt < 3; jCnt++)
		{
			pArray[iCnt][jCnt] = 65 + iCnt * 3 + jCnt;
		}
		pArray[iCnt][3] = 0;
	}

	for (int iCnt = 0; iCnt < 6; iCnt++)
	{
		printf("\n");
		printf("[%d]=%s", iCnt, pArray[iCnt]);
	}

	for (int iCnt = 0; iCnt < 6; iCnt++)
	{
		free(pArray[iCnt]);
	}
	_getch();
}
#else

#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <iostream>
using namespace std;

void main()
{
	cout << "===============" << endl;
	int *pPoint = new int;
	*pPoint = 100;
	cout << hex << pPoint + 0 << "���� : " << dec << *(pPoint + 0) << endl;
	delete pPoint;
	pPoint = 0;

	//////
	cout << "===========" << endl;
	int *pPointArray = new int[10];
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		pPointArray[iCnt] = iCnt * 10;
		cout << pPointArray[iCnt] << "   " << *(pPointArray + iCnt) << endl;
	}
	delete[] pPointArray;

	/////

	cout << "\n====== ���ڸ� �Է��ϰ� ���͸��ÿ�!=======\n" << endl;
	char* pCharPoint = 0;
	int iCount = 0;
	pCharPoint = new char[100];
	do {
		pCharPoint[iCount++] = _getche();
	} while (pCharPoint[iCount - 1] != '\r');

	pCharPoint[iCount + 1] = 0;
	printf("\n0x%08x���� : %s", pCharPoint, pCharPoint);
	for (int iCnt = 0; iCnt < iCount-1; iCnt++)
	{
		printf("\n0x%08X���� : %c", pCharPoint + iCnt, *(pCharPoint + iCnt));
	}
	delete[] pCharPoint;


	/////////

	cout << "\n==================" << endl;
	int iValue[3][2] = { {10, 20}, {30, 40}, {50, 60} };
	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		printf("\n*(iValue+%d): %p", iCnt, *(iValue + iCnt));
		printf("\t\tiValue[%d]: %p", iCnt, iValue[iCnt]);
	}
	cout << "\n" << endl;
	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		for (int jCnt = 0; jCnt < 2; jCnt++)
		{
			printf("\n*(*(iValue+%d)+%d)  : %d", iCnt, jCnt, *(*(iValue + iCnt) + jCnt));
			printf("\t\t *(iValue[%d]+%d) : %d", iCnt, jCnt, *(iValue[iCnt] + jCnt));
		}
	}

	/////

	cout << "\n =======================" << endl;
	int **ppPoint = 0;
	ppPoint = new int*[3];
	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		ppPoint[iCnt] = new int[3];
	}

	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		for (int jCnt = 0; jCnt < 3; jCnt++)
		{
			ppPoint[iCnt][jCnt] = iCnt * 3 + jCnt;
		}
	}

	for (int iCnt = 0; iCnt < 3 ; iCnt++)
	{
		cout << endl;
		for (int jCnt = 0; jCnt < 3; jCnt++)
		{
			cout << "[" << iCnt << "][" << jCnt << "]" << ppPoint[iCnt][jCnt] << ":" << *(ppPoint[iCnt] + jCnt) << *(*(ppPoint + iCnt) + jCnt);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		delete ppPoint[i];
	}
	delete[] ppPoint;

	/////////

	cout << "\n ==============";
	int *lpPoint[10];
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		lpPoint[iCnt] = new int;
		*lpPoint[iCnt] = iCnt * 10;
		printf("\n0x%8X���� : %d", lpPoint + iCnt, *lpPoint[iCnt]);
	}
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		delete lpPoint[iCnt];
	}

	std::cout << "�ƹ�Ű�� �Է��ϸ� ����˴ϴ�." << std::endl;
	_getch();


}
#endif