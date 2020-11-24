#pragma once
#include "sStd.h"
// 1.  게임 실행 시간 누적 기능
// 2. 1프레임 경과 시간 저장
// 3. 1초 동안 프레임 카운터
class sTime
{
public:
	// 게임 실행 시간 누적 하는 함수
	float	m_fGameTimer;
	// 1프레임 경과 시간 저장하는 함수
	float	m_fSecondPerFrame;
	// 1초 동안 프레임 카운터 하는 함수
	int		m_iFPS;

	float fBeforeTime;
public:
	//초기화 및 준비 작업 구현
	bool	Init();
	// 프레임 단위로 계산작업
	bool	Frame();
	// 프레임 단위로 렌더링
	bool	Render();
	// 소멸 및 삭제 기능 구현
	bool	Release();

};

