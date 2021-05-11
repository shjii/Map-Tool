// MapDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "MapDlg.h"
#include "afxdialogex.h"
#include "MFCApplication1.h"

// MapDlg 대화 상자

IMPLEMENT_DYNAMIC(MapDlg, CDialogEx)

MapDlg::MapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MapDlg, pParent)
	, i_Tile(0)
	, i_Cell(0)
	, i_Cellsize(0)
{

}

MapDlg::~MapDlg()
{
}

void MapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, i_Tile);
	DDX_Text(pDX, IDC_EDIT3, i_Cell);
	DDX_Text(pDX, IDC_EDIT4, i_Cellsize);
}


BEGIN_MESSAGE_MAP(MapDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MapDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &MapDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// MapDlg 메시지 처리기


void MapDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.Build(i_Tile, i_Cell, i_Cellsize, a);
	CDialogEx::OnOK();
}


void MapDlg::OnBnClickedButton1()
{
	CFileDialog dlg(
		TRUE, // TRUE for FileOpen, FALSE for FileSaveAs
		L"bmp",
		NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"bmp files(*.bmp)|*.bmp| All Files(*.*)|*.*|",
		this);
	if (dlg.DoModal() == IDOK)
	{
		CString filename;
		filename = dlg.GetPathName();
		filename = dlg.GetFileName();
		//filename += dlg.GetFileExt();
		AfxMessageBox(filename);
		a = filename;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
