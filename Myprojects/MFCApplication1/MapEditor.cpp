// MapEditor.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "MapEditor.h"
#include "afxdialogex.h"


// MapEditor 대화 상자

IMPLEMENT_DYNAMIC(MapEditor, CDialogEx)

MapEditor::MapEditor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SMapEditor, pParent)
	, radius(0)
	, Speed(0)
	, m_Radio(0)
{

}

MapEditor::~MapEditor()
{
}

void MapEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, radius);
	DDX_Text(pDX, IDC_EDIT2, Speed);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio);
}


BEGIN_MESSAGE_MAP(MapEditor, CDialogEx)
	ON_BN_CLICKED(IDOK, &MapEditor::OnBnClickedOk)
END_MESSAGE_MAP()


// MapEditor 메시지 처리기


void MapEditor::OnBnClickedOk()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_EditorData.Speed = Speed;
	theApp.m_Sample.m_EditorData.Radius = radius;
	switch (m_Radio)
	{
	case 0: theApp.m_Sample.m_EditorData.mapEditorB = UP; break;
	case 1: theApp.m_Sample.m_EditorData.mapEditorB = DOWN; break;
	case 2: theApp.m_Sample.m_EditorData.mapEditorB = NORMAL; break;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}
