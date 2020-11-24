#pragma once
#include "sStd.h"

struct sMsg
{
	RECT rt;
	wstring msg;
};

class sWrite
{
private:
	HFONT m_hFont;
	HDC m_hScreenDC;
	vector<sMsg> m_TextList;
public:
	void push(sMsg msg);
public:
	bool Init();
	bool Render();
	bool Release();
	void Draw(int x, int y, wstring msg, DWORD color);
	void Draw(POINT pos, wstring msg, DWORD color);
};


