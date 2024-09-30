// FileOpen.cpp: 구현 파일
//

#include "pch.h"
#include "MDIPainter.h"
#include "FileOpen.h"
#include "afxdialogex.h"


// FileOpen 대화 상자

IMPLEMENT_DYNAMIC(FileOpen, CDialogEx)

FileOpen::FileOpen(CWnd* pParent)
	: CDialogEx(IDD_FILEOPEN, pParent)
{
}

FileOpen::~FileOpen()
{
}

void FileOpen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FileOpen, CDialogEx)
	ON_BN_CLICKED(IDYES, &FileOpen::OnBnClickedYes)
	ON_BN_CLICKED(IDNO, &FileOpen::OnBnClickedNo)
END_MESSAGE_MAP()

// FileOpen 메시지 처리기

void FileOpen::OnBnClickedYes()
{
	m_bClickedYes = true;
	OnCancel();
}
void FileOpen::OnBnClickedNo()
{
	m_bClickedNo = true;
	OnCancel();
}
bool FileOpen::GetClickedYes()
{
	return m_bClickedYes;
}
bool FileOpen::GetClickedNo()
{
	return m_bClickedNo;
}
void FileOpen::SetClickedInit()
{
	m_bClickedYes = m_bClickedNo = false;
}