#pragma once
#include <iostream>

class TRegion
{
public:
	enum
	{
		POINT_POINT, POINT_SIZE
	};
	int m_iWidth;
	int m_iHeight;

private:
	int m_iLeft; // �������� x��ǥ
	int m_iTop; // �������� y ��ǥ
	int m_iRight; // ������ x��ǥ
	int m_iBottom; // ������ y ��ǥ

	static int notation;

public:
	const TRegion* Get() const { return this; }
	const TRegion& GetRef() const { return *this; }

	const int GetLeft() { return m_iLeft; }
	const int GetTop() { return m_iTop; }
	const int *GetRight() { return &m_iRight; }
	const int &GetBottom() { return m_iBottom; }

	int GetWidth() const;
	const int GetHeight() const;
	void GetStartPoint(int &x, int &y) const;
	void GetEndPoint(int &x, int &y)const;
	void GetCenterPoint(int &x, int &y) const;
	bool IsPointInRegion(int x, int y) const;

	void SetRect(int l, int t, int r, int b);
	void Move(int x, int y);
	void Resize(int width, int height);
	void UnionRegion(const TRegion &r1, const TRegion &r2);
	bool IntersectRegion(const TRegion &r1, const TRegion &r2);

	void Show(void);
	static void SetNotation(int notation);
public:
	TRegion(void);
	TRegion(int l, int t, int r, int b);
};
