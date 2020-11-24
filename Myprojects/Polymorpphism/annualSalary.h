#pragma once
#include "employee.h"

class annualSalary : public employee
{// ¿¬ºÀ
public:

	void Show();

	int iVaule = 0;

	int Rating = 0;

	annualSalary(const char* name,char position,int Rating);
	~annualSalary();

};

