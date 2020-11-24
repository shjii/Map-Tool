#pragma once
#include <iostream>

class employee
{
public:

	char name[20];

	enum position
	{
		Staff,// 사원
		Assistant, // 대리
		Supervisor, // 과장
		Manager, // 부장
		Director // 이사
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

