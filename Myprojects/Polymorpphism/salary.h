#pragma once
#include "employee.h"

class salary : public employee
{// ПљБо
public:

	void Show();

	int iVaule = 0;

	int salaryYear = 0;


	salary(const char* name, char position, int salaryYear);

	~salary();
};

