#include "annualSalary.h"

void annualSalary::Show()
{
	iVaule = RatingMethod(i_position, Rating);

	std::cout << "이름 :" << name << "직급 :" << "급여 산출 방식 : 연봉제" << "능력 : "<< i_position << "급"<< "월급" << iVaule << "만원" << std::endl;
}

annualSalary::annualSalary(const char* name, char position, int Rating)
{
	strcpy_s(this->name, name);

	i_position = position;

	this->Rating = Rating;
}

annualSalary::~annualSalary()
{
	std::cout << "annualSalary 소멸자" << std::endl;
}