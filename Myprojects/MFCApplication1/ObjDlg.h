#pragma once


// ObjDlg 대화 상자

class ObjDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ObjDlg)

public:
	ObjDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ObjDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ObjLog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	CListBox m_FBXList;
	afx_msg void OnBnNewList();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnObjAdd();
	afx_msg void OnBnObjDel();
	afx_msg void OnBnClickedButton3();
};
