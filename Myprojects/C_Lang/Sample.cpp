#define _CRT_SECURE_NO_WARNINGS
#define SAMPLE_01
//#undef SAMPLE_01
#define CPLUSPLUS
#undef CPLUSPLUS

#ifdef SAMPLE_01

#ifndef CPLUSPLUS

#include <stdio.h>
void main()
{
	printf("======= CODE 002 ====== \n");
	int iValue;
	float fValue;
	char strValue[20];
	char cValue1, cValue2;
	printf("\n 정수 및 실수를 입력하시오 =>");
	scanf("%d %f", &iValue, &fValue);

	printf("\n 문자열을 입력하시오 =>");
	scanf("%s", strValue);

	printf("\n iValue = %d", iValue);
	printf("\n fValue  = %f", fValue);
	printf("\n strValue  = %s", strValue);

	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	printf("\n 문자 1을 입력하시오");
	scanf("%c", &cValue1);
	printf("\n strValue = %s, %c ", strValue, cValue1);
}
#else

#include <cstdio>
#include <iostream>
using namespace std;

void main()
{
	printf("======== CODE 002 ======= \n");
	int iValue;
	float FValue;
	char strValue[20];
	char cValue1, cValue2;
	cout << "정수와 실수 입력하시오 =>" << endl;

}
#endif
#endif