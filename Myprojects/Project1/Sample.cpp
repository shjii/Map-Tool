#define SAMPLE

#ifdef SAMPLE
#include <stdio.h>
void main()
{
	int iValue = 10;

	printf("%s:%d\n", "kgca", iValue);
}

#else

#include <iostream>

void main()
{
	std::cout << "������" << std::endl;
}
#endif