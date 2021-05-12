// SMapToolForm.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "SMapToolForm.h"


// SMapToolForm

IMPLEMENT_DYNCREATE(SMapToolForm, CFormView)

SMapToolForm::SMapToolForm()
	: CFormView(IDD_SMapToolForm)
{

}

SMapToolForm::~SMapToolForm()
{
}

void SMapToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(SMapToolForm, CFormView)
END_MESSAGE_MAP()


// SMapToolForm 진단

#ifdef _DEBUG
void SMapToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void SMapToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// SMapToolForm 메시지 처리기
