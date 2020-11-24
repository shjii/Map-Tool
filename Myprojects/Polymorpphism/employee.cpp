#include "employee.h"


void employee::Show()
{
	std::cout << "부모 함수 " << std::endl;
}

int employee::hourlyWageTimeMethod(int position, int hourlyWageTime)
{
	int a = 1;

	for (int i = 0; i < position; i++)
	{
		a = a * 2;
	}
	
	int b = hourlyWageTime * a;

	return b;
}

int employee::salaryYearMethod(int position, int salaryYear)
{
	int a = 200;

	int b = a + (20 * position + (salaryYear * 5));

	return b;
}

int employee::RatingMethod(int position, int Rating)
{
	int a = 2000;

	int b = a + (1000 * position + (Rating * 500));

	b = b / 12;

	return b;
}

employee::employee(char* _name, int position, int x)
{

}

employee::employee()
{

}

employee::~employee()
{

}