#include "sCollision.h"

bool sCollision::RectInpt(RECT rt, POINT pt)
{
		return (pt.x >= rt.left &&
			pt.x <= rt.left + rt.right &&
			pt.y >= rt.top &&
			pt.y <= rt.top + rt.bottom);
}