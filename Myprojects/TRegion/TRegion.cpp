#include "TRegion.h"

int TRegion::notation = TRegion::POINT_POINT;

TRegion::TRegion(void)
{
	SetRect(0, 0, 0, 0);
}

TRegion::TRegion(int l, int t, int r, int b)
{
	SetRect(l, t, r, b);
}

void TRegion::SetRect(int l, int t, int r, int b)
{
	m_iLeft = l;
	m_iTop = t;
	m_iRight = r;
	m_iBottom = b;
}

int TRegion::GetWidth() const
{
	return m_iRight - m_iLeft;
}

const int TRegion::GetHeight() const
{
	return m_iBottom - m_iTop;
}

void TRegion::GetStartPoint(int &x, int &y) const
{
	x = m_iLeft;
	y = m_iTop;
}

void TRegion::GetEndPoint(int &x, int &y) const
{
	x = m_iRight;
	y = m_iBottom;
}

void TRegion::GetCenterPoint(int &x, int &y) const
{
	x = (m_iLeft + m_iRight) / 2;	
	y = (m_iTop + m_iBottom) / 2;	
}

bool TRegion::IsPointInRegion(int x, int y) const
{
	return (x >= m_iLeft && x <= m_iRight && y >= m_iTop && y <= m_iBottom);
}

void TRegion::Move(int x, int y)
{
	m_iLeft += x;
	m_iTop += y;
	m_iRight += x;
	m_iBottom += y;
}

void TRegion::Resize(int width, int height)
{
	int x, y;
	GetCenterPoint(x, y);
	m_iLeft = x - width / 2;
	m_iTop = y - height / 2;	
	m_iRight = m_iLeft + width;	
	m_iBottom = m_iTop + height;
}

void TRegion::UnionRegion(const TRegion &r1, const TRegion &r2)
{
	m_iLeft = r1.m_iLeft < r2.m_iLeft ? r1.m_iLeft : r2.m_iLeft;
	m_iRight = r1.m_iRight > r2.m_iRight ? r1.m_iRight : r2.m_iRight;
	m_iTop = r1.m_iTop < r2.m_iTop ? r1.m_iTop : r2.m_iTop;
	m_iBottom = r1.m_iBottom > r2.m_iBottom ? r1.m_iBottom : r2.m_iBottom;
}

bool TRegion::IntersectRegion(const TRegion &r1, const TRegion &r2)
{
	m_iLeft = r1.m_iLeft > r2.m_iLeft ? r1.m_iLeft : r2.m_iLeft;
	m_iRight = r1.m_iRight < r2.m_iRight ? r1.m_iRight : r2.m_iRight;
	m_iTop = r1.m_iTop > r2.m_iTop ? r1.m_iTop : r2.m_iTop;
	m_iBottom = r1.m_iBottom < r2.m_iBottom ? r1.m_iBottom : r2.m_iBottom;

	if (m_iLeft >= m_iRight || m_iTop >= m_iBottom)
	{
		m_iLeft = m_iTop = m_iRight = m_iBottom = 0;
		return false;
	}
	return true;
}

void TRegion::SetNotation(int n)
{
	notation = n;
}

void TRegion::Show(void)
{
	if (notation == POINT_POINT)	// 시작점/끝점 형식
	{
		printf("(%d, %d), (%d, %d)\n", m_iLeft, m_iTop, m_iRight, m_iBottom);
	}
	else					// 시작점/가로, 세로/길이 형식
	{
		printf("(%d, %d), [%d x %d]\n", m_iLeft, m_iTop, GetWidth(), GetHeight());
	}
}
