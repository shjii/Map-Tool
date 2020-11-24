#pragma once
#include <iostream>
using namespace std;

class TPoint
{
public :
	int x, y;
	void SetPosirion(int _x, int _y);
	void Move(int _x, int _y);
	void Show(void);

	TPoint* operator->() {
		return this;
	}
	TPoint& operator*() {
		return *this;
	}

	TPoint operator++(void);
	TPoint operator++(int);

	TPoint operator+(const TPoint &pt);
	TPoint operator-(const TPoint &pt);
	TPoint operator*(const TPoint &c);
	TPoint operator/(const TPoint &c);

	TPoint operator*(int mag) const;
	TPoint operator/(int div) const;

	friend TPoint operator*(int mag, const TPoint &pt);
	friend TPoint operator/(int div, const TPoint &pt);

	TPoint &operator=(const TPoint &pt);
	TPoint &operator+=(const TPoint &pt);
	TPoint &operator-=(const TPoint &pt);
	TPoint &operator*=(const TPoint &pt);
	TPoint &operator/=(const TPoint &pt);
	TPoint &operator*=(int mag);
	TPoint &operator/=(int div);

	bool operator==(const TPoint &pt);
	bool operator!=(const TPoint &pt);

	int &operator[](int index);
	int &operator()(int index);

	void *operator new(size_t size);
	void operator delete(void *p);

	friend ostream &operator<<(ostream &os, const TPoint &pt);
	friend istream &operator>>(istream &is, TPoint &pt);

	void (TPoint::*m_fp)();
	void Execute() {
		if (m_fp != NULL)
		{
			(this->*m_fp)();
		}
	}
	void operator > (TPoint* the)
	{
		if (m_fp != NULL)
		{
			(the->*m_fp)();
		}
	}

	TPoint();
	TPoint(int x, int y);
	~TPoint();
};

