#pragma once
#include "sObject.h"
class sEcentScene : public sObject
{
public:
	int stage;
	int NPCcount;

	int Human;
	int Tank;
	int Boss;

public:
	int Event(int a);
};

