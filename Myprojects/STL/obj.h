#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <string>

using namespace std;

class CObj {
public:
	CObj() {};
	CObj(string _name) {
		szName = _name;
	};
	virtual ~CObj() {};
	virtual string GetName() = 0;

protected:
	string szName;
};

class CItem : public CObj
{
public:
	int iTemNumber;

	CItem() {};
	CItem(string _name, int _iTemNumber)
		: CObj(_name) {
		szName = _name;
		iTemNumber = _iTemNumber;
	};
	virtual ~CItem() {};

	virtual string GetName() {
		return szName;
	}

};

class Weapon : public CItem
{
public:
	int offensivePower;

	Weapon(string _name, int _iTemNumber, int _offensivePower) : CItem(_name, _iTemNumber) {
		szName = _name;
		iTemNumber = _iTemNumber;
		offensivePower = _offensivePower;
	}
	Weapon() {}

	string getname()
	{
		return szName;
	}
};