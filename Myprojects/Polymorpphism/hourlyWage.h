#pragma once
#include "employee.h"

class hourlyWage : public employee
{// �ñ�
public:

	void Show();

	int iVaule = 0;

	int hourlyWageTime = 0;

	hourlyWage(const char* name, char position, int hourlyWageTime);

	~hourlyWage();
};

