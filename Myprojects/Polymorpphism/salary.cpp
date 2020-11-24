#include "salary.h"

void salary::Show()
{
	iVaule = salaryYearMethod(i_position, salaryYear);

	std::cout << "이름 :" << name << "직급 :" << "급여 산출 방식 : 연봉" << "능력 : " << i_position << "시간" << "월급" << iVaule << "만원" << std::endl;
}

salary::salary(const char* name, char position, int salaryYear)
{
	strcpy_s(this->name, name);

	i_position = position;

	this->salaryYear = salaryYear;
}

salary::~salary()
{
	std::cout << "hourlyWageTime 소멸자" << std::endl;
}