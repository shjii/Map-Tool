#include "annualSalary.h"
#include "hourlyWage.h"
#include "salary.h"

int main()
{
	employee *Money[10] =
	{
		new annualSalary("������", 0, 10),
		new hourlyWage("������", 3, 5),
		new salary("�̴ٿ�", 2, 4)
	};

	for (int i = 0; i < 3; i++)
	{
		Money[i]->Show();
	}

	for (int i = 0; i < 3; i++)
	{
		delete Money[i];
	}
}