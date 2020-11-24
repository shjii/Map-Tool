#pragma once
#include "employee.h"

class hourlyWage : public employee
{// ½Ã±Þ
public:

	void Show();

	int iVaule = 0;

	int hourlyWageTime = 0;

	hourlyWage(const char* name, char position, int hourlyWageTime);

	~hourlyWage();
};

