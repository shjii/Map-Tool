#pragma once


// MapDlg 대화 상자

class MapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MapDlg)

public:
	MapDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MapDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MapDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int i_Tile;
	int i_Cell;
	int i_Cellsize;
	wstring a;
	BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
