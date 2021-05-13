#pragma once


// MapEditor 대화 상자

class MapEditor : public CDialogEx
{
	DECLARE_DYNAMIC(MapEditor)

public:
	MapEditor(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MapEditor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMapEditor };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_iSubRadio;
	float radius;
	float Speed;
	afx_msg void OnBnClickedOk();
	int m_Radio;
};
