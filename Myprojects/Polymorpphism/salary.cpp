#include "salary.h"

void salary::Show()
{
	iVaule = salaryYearMethod(i_position, salaryYear);

	std::cout << "�̸� :" << name << "���� :" << "�޿� ���� ��� : ����" << "�ɷ� : " << i_position << "�ð�" << "����" << iVaule << "����" << std::endl;
}

salary::salary(const char* name, char position, int salaryYear)
{
	strcpy_s(this->name, name);

	i_position = position;

	this->salaryYear = salaryYear;
}

salary::~salary()
{
	std::cout << "hourlyWageTime �Ҹ���" << std::endl;
}