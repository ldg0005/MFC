// ViewClose.cpp: 구현 파일
//

#include "pch.h"
#include "MDIPainter.h"
#include "ViewClose.h"
#include "afxdialogex.h"


// ViewClose 대화 상자

IMPLEMENT_DYNAMIC(ViewClose, CDialogEx)

ViewClose::ViewClose(CWnd* pParent)
	: CDialogEx(IDD_VIEWCLOSE, pParent)
{
}

ViewClose::~ViewClose()
{
}

void ViewClose::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ViewClose, CDialogEx)
	ON_BN_CLICKED(IDYES, &ViewClose::OnBnClickedYes)
	ON_BN_CLICKED(IDNO, &ViewClose::OnBnClickedNo)
END_MESSAGE_MAP()


// ViewClose 메시지 처리기

void ViewClose::OnBnClickedYes()
{
	m_bClickedYes = true;
	OnCancel();
}
void ViewClose::OnBnClickedNo()
{
	m_bClickedNo = true;
	OnCancel();
}
bool ViewClose::GetClickedYes() 
{
	return m_bClickedYes; 
}
bool ViewClose::GetClickedNo() 
{
	return m_bClickedNo; 
}
void ViewClose::SetClickedInit() 
{
	m_bClickedYes = m_bClickedNo = false;
}