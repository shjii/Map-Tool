#include "hourlyWage.h"

#include "annualSalary.h"

void hourlyWage::Show()
{
	iVaule = hourlyWageTimeMethod(i_position, hourlyWageTime);

	std::cout << "�̸� :" << name << "���� :" << "�޿� ���� ��� : �ñ���" << "���� : " << i_position << "��" << "����" << iVaule << "����" << std::endl;
}

hourlyWage::hourlyWage(const char* name, char position, int hourlyWageTime)
{
	strcpy_s(this->name, name);

	i_position = position;

	this->hourlyWageTime = hourlyWageTime;
}

hourlyWage::~hourlyWage()
{
	std::cout << "hourlyWageTime �Ҹ���" << std::endl;
}