
#include "pch.h"
#include "MDIPainter.h"
#include "FileClose.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(FileClose, CDialogEx)

FileClose::FileClose(CWnd* pParent)
	: CDialogEx(IDD_CLOSE, pParent)
{
}
FileClose::~FileClose()
{
}
void FileClose::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FileClose, CDialogEx)
	ON_BN_CLICKED(IDYES, &FileClose::OnBnClickedYes)
END_MESSAGE_MAP()

void FileClose::OnBnClickedYes()
{
	m_bClickedYes = true;
	OnCancel();
}
