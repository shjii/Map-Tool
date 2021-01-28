#include "main.h"
bool main::Init()
{
	m_Device.Init();
	return true;
}
bool main::Frame()
{
	return true;
}
bool main::PreRender()
{
	m_Device.PreRender();
	return true;
}
bool main::Render()
{
	m_Device.Render();
	return true;
}
bool main::PostRender()
{
	m_Device.PostRender();
	return true;
}
bool main::Release()
{
	return true;
}

GAMERUN;