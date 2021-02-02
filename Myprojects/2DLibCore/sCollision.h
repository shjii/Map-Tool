#pragma once
#include "sStd.h"
class sCollision
{
public:
	static bool RectInpt(RECT rt, POINT pt);
	static bool Rect2Rect(RECT rtDesk, RECT rtSrc);
};

