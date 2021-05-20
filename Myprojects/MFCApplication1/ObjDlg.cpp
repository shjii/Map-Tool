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
	, SX(0)
	, SY(0)
	, SZ(0)
	, QX(0)
	, QY(0)
	, QZ(0)
{

}

ObjDlg::~ObjDlg()
{
}

void ObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FBX_LIST, m_FBXList);
	DDX_Text(pDX, IDC_EDIT1, SX);
	//DDX_Text(pDX, IDC_EDIT5, SY);
	//DDX_Text(pDX, IDC_EDIT6, SZ);
	//DDX_Text(pDX, IDC_EDIT7, QX);
	//DDX_Text(pDX, IDC_EDIT8, QY);
	//DDX_Text(pDX, IDC_EDIT9, QZ);
	DDX_Text(pDX, IDC_EDIT2, SY);
	DDX_Text(pDX, IDC_EDIT3, SZ);
	DDX_Text(pDX, IDC_EDIT4, QX);
	DDX_Text(pDX, IDC_EDIT5, QY);
	DDX_Text(pDX, IDC_EDIT6, QZ);
}


BEGIN_MESSAGE_MAP(ObjDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &ObjDlg::OnBnNewList)
	ON_BN_CLICKED(IDC_BUTTON5, &ObjDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &ObjDlg::OnBnObjAdd)
	ON_BN_CLICKED(IDC_BUTTON2, &ObjDlg::OnBnObjDel)
	ON_BN_CLICKED(IDC_BUTTON3, &ObjDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &ObjDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ObjDlg 메시지 처리기
BOOL ObjDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SX = SY = SZ = 1.0f;
	m_FBXList.AddString(L"Turret_Deploy1.fbx");
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


void ObjDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	Vector3 a = {SX, SY, SZ};
	Quaternion b = {QX, QY, QZ, 1.0f};
	theApp.m_Sample.setSRT(a, b);
	int c = m_FBXList.GetCurSel();
	CString st;
	m_FBXList.GetText(c,st);
	wstring qwe = L"../../data/3DS/";
	qwe += st;
	theApp.m_Sample.objname = qwe;
	//UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialogEx::OnOK();
}
