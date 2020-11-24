#pragma once
#include "sScene.h"
class sSceneLobby :
	public sScene
{
public:
	float t;
	bool c;
	int a;
	sPoint P1[4] = { {140.0f, 130.0f} , {430.0f, 130.0f}, {720.0f,130.0f},{1010.0f, 130.0f} };
	sPoint CW[4] = { {80.0f, 190.0f} , {370.0f ,190.0f}, {655.0f ,190.0f},{945.0f ,190.0f} };
	sPoint CC[4] = { {80.0f, 190.0f} , {370.0f, 190.0f}, {655.0f, 190.0f},{945.0f, 190.0f} };
	bool	Frame();
	bool ReSet()override;
	sSceneLobby() { a = 0; 
	t = 0.0f;
	c = false;
	}
};

