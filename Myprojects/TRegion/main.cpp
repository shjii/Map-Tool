#include "TRegion.h"

int main()
{
	TRegion::SetNotation(TRegion::POINT_POINT);
	TRegion r1(10, 10, 100, 100);
	TRegion r2(50, 50, 120, 120);

	const TRegion* x1 = r1.Get();
	const TRegion& x2 = r2.GetRef();

	int ix = x1->GetWidth();
	int iT = x1->GetHeight();

	const int iC = x1->GetWidth();
	const int iD = x1->GetHeight();

	int iXref = x2.GetWidth();
	int iY = r1.GetLeft();
	int iZ = r1.GetTop();
	const int iG = r1.GetLeft();
	const int iJ = r1.GetTop();
	const int* iQ = r1.GetRight();
	const int& i7 = r1.GetBottom();

	TRegion r3;
	r3.IntersectRegion(r1, r2);	r3.Show();
	r3.UnionRegion(r1, r2);		r3.Show();
	r3.Move(100, 100);			r3.Show();
	r3.Resize(200, 200);		r3.Show();

	TRegion::SetNotation(TRegion::POINT_SIZE);
	r1.Show();
	r2.Show();
	r3.Show();
	return 0;
}