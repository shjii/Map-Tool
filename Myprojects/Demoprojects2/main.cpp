#include "main.h"

bool	main::Init()
{
	sTime.Init();
	return true;
}
bool	main::Frame()
{
	sTime.Frame();
	return true;
}
bool	main::Render()
{
	sTime.Render();
	return true;
}
bool	main::Release()
{
	sTime.Release();
	return true;
}

GAMERUN;