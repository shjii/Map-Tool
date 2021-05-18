// ObjDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "ObjDlg.h"
#include "afxdialogex.h"


// ObjDlg 대화 상자

IMPLEMENT_DYNAMIC(ObjDlg, CDialogEx)

ObjDlg::ObjDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ObjLog, pParent)
{

}

ObjDlg::~ObjDlg()
{
}

void ObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FBX_LIST, m_FBXList);
}


BEGIN_MESSAGE_MAP(ObjDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &ObjDlg::OnBnNewList)
	ON_BN_CLICKED(IDC_BUTTON5, &ObjDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &ObjDlg::OnBnObjAdd)
	ON_BN_CLICKED(IDC_BUTTON2, &ObjDlg::OnBnObjDel)
	ON_BN_CLICKED(IDC_BUTTON3, &ObjDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// ObjDlg 메시지 처리기
BOOL ObjDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_FBXList.AddString(L"alsdkfj");
	UpdateData(FALSE);
	return TRUE;
}


void ObjDlg::OnBnNewList()
{
	CFileDialog dlg(
		TRUE, // TRUE for FileOpen, FALSE for FileSaveAs
		L"fbx",
		NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"fbx files(*.fbx)|*.fbx| All Files(*.*)|*.*|",
		this);
	if (dlg.DoModal() == IDOK)
	{
		CString filename;
		filename = dlg.GetPathName();
		filename = dlg.GetFileName();
		//filename += dlg.GetFileExt();
		AfxMessageBox(filename);
		m_FBXList.AddString(filename);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void ObjDlg::OnBnClickedButton5()
{
	int a = m_FBXList.GetCurSel();
	m_FBXList.DeleteString(a);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ObjDlg::OnBnObjAdd()
{
	theApp.m_Sample.objp = true;
	theApp.m_Sample.objEd = 0;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ObjDlg::OnBnObjDel()
{
	theApp.m_Sample.objp = true;
	theApp.m_Sample.objEd = 1;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ObjDlg::OnBnClickedButton3()
{
	theApp.m_Sample.objp = true;
	theApp.m_Sample.objEd = 2;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
