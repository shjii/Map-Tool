#pragma once

struct SVector2
{
	float  x;
	float  y;
	SVector2() {}
	SVector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};
struct SVector3
{
	float  x;
	float  y;
	float  z;
	SVector3() {}
	SVector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};
struct SVector4
{
	float  x;
	float  y;
	float  z;
	float  w;
	SVector4() {}
	SVector4(float x, float y, float  w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};