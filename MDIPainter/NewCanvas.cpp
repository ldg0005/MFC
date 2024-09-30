// CNewCanvas.cpp: 구현 파일
//

#include "pch.h"
#include "MDIPainter.h"
#include "NewCanvas.h"
#include "afxdialogex.h"


// CNewCanvas 대화 상자

IMPLEMENT_DYNAMIC(NewCanvas, CDialogEx)

NewCanvas::NewCanvas(CWnd* pParent)
	: CDialogEx(IDD_NEWCANVAS, pParent)
{
}
NewCanvas::~NewCanvas()
{
}
void NewCanvas::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(NewCanvas, CDialogEx)
	ON_BN_CLICKED(IDYES, &NewCanvas::OnBnClickedYes)
	ON_BN_CLICKED(IDNO, &NewCanvas::OnBnClickedNo)
END_MESSAGE_MAP()


// CNewCanvas 메시지 처리기

void NewCanvas::OnBnClickedYes()
{
	m_bClickedYes = true;
	OnCancel();
}
void NewCanvas::OnBnClickedNo()
{
	m_bClickedNo = true;
	OnCancel();
}
bool NewCanvas::GetClickedYes()
{ 
	return m_bClickedYes; 
}
bool NewCanvas::GetClickedNo()
{
	return m_bClickedNo; 
}
void NewCanvas::SetClickedInit()
{
	m_bClickedYes = m_bClickedNo = false; 
}