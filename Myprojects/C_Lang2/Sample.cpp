//#define CPLUSPLUS

#ifndef CPLUSPLUS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <malloc.h>
#else
#include <iostream>
#include <conio.h>
using namespace std;
#endif

int Compute2(int iValueX, int iValyeY)
{
	int iAdd = iValueX + iValyeY;
	//iValueX = iAdd;
	return iAdd;
}

int Compute(int *iValueX, int *iValueY)
{
	int iAdd = *iValueX + *iValueY;
	//*iValueX = iAdd;
	return iAdd;
}

void ComputeArray(int iList[], int n)
{
	for (int iCnt = 0; iCnt < n; iCnt++)
	{
		iList[iCnt] += 3;
	}
}

void ComputeArray2(int *iList, int n)
{
	for (int iCnt = 0; iCnt < n; iCnt++)
	{
		iList[iCnt] += 3;
	}
}

char* StringToUpper(const char *strList)
{
	int iCount = 0;
	while (strList[iCount++] != 0);

	char* pString = (char*)malloc(sizeof(char)*(iCount));

	int iCnt = 0;

	for (iCnt = 0; iCnt < iCount - 1; iCnt++)
	{
		pString[iCnt] = toupper(strList[iCnt]);
	}

	pString[iCount - 1] = 0;
	return pString;
}
void main()
{
	int iX = 10;
	int iY = 20;
	int iArray[] = { 10,20,30,40,50,60,70,80,90,100 };
	int iSize1 = sizeof(iArray);
	int iSize2 = sizeof(iArray) / sizeof(iArray[0]);
	int iCount = _countof(iArray);
#ifdef  CPLUSPLUS
	cout << Compute(&iX, &iY) << endl;
	cout << iX << iY << endl;
#else
	printf("\n iX+iY : %d", Compute2(iX, iY));
	printf("\n iX : %d , iY : %d\n", iX, iY);
#endif //  CPLUSPLUS

	int iMaxCnt = sizeof(iArray) / sizeof(iArray[0]);
	int iTotalSize = sizeof(iArray);

	ComputeArray(iArray, iMaxCnt);

	for (int iCnt = 0; iCnt < iMaxCnt; iCnt++)
	{
#ifndef CPLUSPLUS
		printf("%5d\n", iArray[iCnt]);
#else
		cout << iArray[iCnt] << endl;
#endif // !CPLUSPLUS
	}
	char *p = StringToUpper("abcde FGHIJ");
	int iMallocCount = _msize(p) / sizeof(char);
#ifndef CPLUSPLSU
	printf("%5d\n", iMallocCount);
#else
	cout << iMallocCount << endl;
#endif // !CPLUSPLSU
	puts(p);
	free(p);
	_getch();
}

