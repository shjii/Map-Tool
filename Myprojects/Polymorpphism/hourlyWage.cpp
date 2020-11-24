#include "hourlyWage.h"

#include "annualSalary.h"

void hourlyWage::Show()
{
	iVaule = hourlyWageTimeMethod(i_position, hourlyWageTime);

	std::cout << "이름 :" << name << "직급 :" << "급여 산출 방식 : 시급제" << "연차 : " << i_position << "년" << "월급" << iVaule << "만원" << std::endl;
}

hourlyWage::hourlyWage(const char* name, char position, int hourlyWageTime)
{
	strcpy_s(this->name, name);

	i_position = position;

	this->hourlyWageTime = hourlyWageTime;
}

hourlyWage::~hourlyWage()
{
	std::cout << "hourlyWageTime 소멸자" << std::endl;
}