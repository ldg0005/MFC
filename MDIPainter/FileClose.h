#pragma once

class FileClose : public CDialogEx
{
	DECLARE_DYNAMIC(FileClose)

public:
	FileClose(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~FileClose();
	bool m_bClickedYes = false;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLOSE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedYes();
};
