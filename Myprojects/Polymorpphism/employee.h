#pragma once
#include <iostream>

class employee
{
public:

	char name[20];

	enum position
	{
		Staff,// ���
		Assistant, // �븮
		Supervisor, // ����
		Manager, // ����
		Director // �̻�
	};
	int i_position = 99;

	virtual void Show();

	int hourlyWageTimeMethod(int position, int hourlyWageTime);
	
	int salaryYearMethod(int position, int salaryYear);

	int RatingMethod(int position, int Rating);

	employee(char* _name, int position, int x);

	employee();
	
	virtual~employee();
};

