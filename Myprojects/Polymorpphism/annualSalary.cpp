#include "annualSalary.h"

void annualSalary::Show()
{
	iVaule = RatingMethod(i_position, Rating);

	std::cout << "�̸� :" << name << "���� :" << "�޿� ���� ��� : ������" << "�ɷ� : "<< i_position << "��"<< "����" << iVaule << "����" << std::endl;
}

annualSalary::annualSalary(const char* name, char position, int Rating)
{
	strcpy_s(this->name, name);

	i_position = position;

	this->Rating = Rating;
}

annualSalary::~annualSalary()
{
	std::cout << "annualSalary �Ҹ���" << std::endl;
}