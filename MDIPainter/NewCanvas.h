#pragma once


// NewCanvas 대화 상자

class NewCanvas : public CDialogEx
{
	DECLARE_DYNAMIC(NewCanvas)

public:
	NewCanvas(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~NewCanvas();
	bool GetClickedYes();
	bool GetClickedNo();
	void SetClickedInit();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWCANVAS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	bool m_bClickedYes = false;
	bool m_bClickedNo = false;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedNo();
};
