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
	printf("\n ���� �� �Ǽ��� �Է��Ͻÿ� =>");
	scanf("%d %f", &iValue, &fValue);

	printf("\n ���ڿ��� �Է��Ͻÿ� =>");
	scanf("%s", strValue);

	printf("\n iValue = %d", iValue);
	printf("\n fValue  = %f", fValue);
	printf("\n strValue  = %s", strValue);

	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	printf("\n ���� 1�� �Է��Ͻÿ�");
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
	cout << "������ �Ǽ� �Է��Ͻÿ� =>" << endl;

}
#endif
#endif