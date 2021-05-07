// SMapDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "SMapDlg.h"
#include "afxdialogex.h"


// SMapDlg 대화 상자

IMPLEMENT_DYNAMIC(SMapDlg, CDialogEx)

SMapDlg::SMapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

SMapDlg::~SMapDlg()
{
}

void SMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SMapDlg, CDialogEx)
END_MESSAGE_MAP()


// SMapDlg 메시지 처리기
