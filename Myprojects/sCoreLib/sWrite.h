#pragma once
#include "sStd.h"

struct sMsg
{
	RECT rt;
	wstring msg;
};

class sWrite : public GetSing<sWrite>
{
private:
	HFONT m_hFont;
	//HDC m_hScreenDC;
	vector<sMsg> m_TextList;
public:
	void push(sMsg msg);
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	void   Draw(POINT pos, wstring msg, DWORD color = RGB(255, 0, 0));
	void   Draw(int x, int y, wstring msg, DWORD color = RGB(255, 0, 0));
public:
	sWrite();
	virtual ~sWrite();
};
#define g_Write sWrite::GetInstance()

