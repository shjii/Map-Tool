#pragma once
#include "sStd.h"

struct sMsg
{
	RECT rt;
	wstring msg;
};

class sWrite : public GetSing<sWrite>
{
public:
	HFONT m_hFont;
	//HDC m_hScreenDC;
	vector<sMsg> m_TextList;
	sMsg m_chText;
	bool keyc;
public:
	void push(sMsg msg);
	void chpush(sMsg msg);
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	void   Draw(POINT pos, wstring msg, DWORD color = RGB(255, 0, 0));
	void   Draw(int x, int y, wstring msg, DWORD color = RGB(255, 0, 0));
	bool	chc(wstring& a);
public:
	sWrite();
	virtual ~sWrite();
};
#define g_Write sWrite::GetInstance()

