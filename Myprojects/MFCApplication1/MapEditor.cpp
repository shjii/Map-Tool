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
	ON_BN_CLICKED(IDC_BUTTON1, &MapEditor::OnBnClickedButtonL0)
	ON_BN_CLICKED(IDC_BUTTON2, &MapEditor::OnBnClickedButtonL1)
	ON_BN_CLICKED(IDC_BUTTON3, &MapEditor::OnBnClickedButtonL2)
	ON_BN_CLICKED(IDC_BUTTON4, &MapEditor::OnBnClickedButtonL3)
	ON_BN_CLICKED(IDC_BUTTON5, &MapEditor::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON6, &MapEditor::OnBnClickedLoad)
END_MESSAGE_MAP()


// MapEditor 메시지 처리기


void MapEditor::OnBnClickedOk()
{
	UpdateData(TRUE);
	theApp.m_Sample.objp = false;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_EditorData.Speed = Speed;
	theApp.m_Sample.m_EditorData.Radius = radius;
	switch (m_Radio)
	{
	case 0: theApp.m_Sample.m_EditorData.mapEditorB = UP; break;
	case 1: theApp.m_Sample.m_EditorData.mapEditorB = DOWN; break;
	case 2: theApp.m_Sample.m_EditorData.mapEditorB = NORMAL; break;

	case 3 :theApp.m_Sample.m_EditorData.mapEditorB = ELayer0; break;
	case 4:theApp.m_Sample.m_EditorData.mapEditorB = ELayer1; break;
	case 5:theApp.m_Sample.m_EditorData.mapEditorB = ELayer2; break;
	case 6:theApp.m_Sample.m_EditorData.mapEditorB = ELayer3; break;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}

BOOL MapEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Speed = 5.0f;
	radius = 10.0f;

	UpdateData(FALSE);
	return TRUE;
}

void MapEditor::OnBnClickedButtonL0()
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
		theApp.m_Sample.m_MapData.m_LayerList[0] = filename;
		theApp.m_Sample.SetLayer();
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void MapEditor::OnBnClickedButtonL1()
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
		theApp.m_Sample.m_MapData.m_LayerList[1] = filename;
		theApp.m_Sample.SetLayer();
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MapEditor::OnBnClickedButtonL2()
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
		theApp.m_Sample.m_MapData.m_LayerList[2] = filename;
		theApp.m_Sample.SetLayer();
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MapEditor::OnBnClickedButtonL3()
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
		theApp.m_Sample.m_MapData.m_LayerList[3] = filename;
		theApp.m_Sample.SetLayer();
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MapEditor::OnBnClickedSave()
{
	//CFileDialog dlg(
	//	FALSE, // TRUE for FileOpen, FALSE for FileSaveAs
	//	L"bmp",
	//	NULL,
	//	OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
	//	L"bmp files(*.bmp)|*.bmp| All Files(*.*)|*.*|",
	//	this);
	//theApp.m_Sample.m_MapData.btext = theApp.m_Sample.m_BlendingTextrue.pStaging2D.Get();
	theApp.m_Sample.m_MapData.btext = theApp.m_Sample.m_BlendingTextrue.pTex2D.Get();
	theApp.m_Sample.m_MapData.ResourceView = theApp.m_Sample.m_BlendingTextrue.m_pSRV.Get();
	theApp.m_Sample.FileIO.Save(theApp.m_Sample.m_MapData, theApp.m_Sample.m_pd3dDevice.Get(),theApp.m_Sample.m_pImmediateContext.Get());

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MapEditor::OnBnClickedLoad()
{
	theApp.m_Sample.SetEditor();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
