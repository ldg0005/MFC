﻿#pragma once


// ViewClose 대화 상자

class ViewClose : public CDialogEx
{
	DECLARE_DYNAMIC(ViewClose)

public:
	ViewClose(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ViewClose();
	bool GetClickedYes(); 
	bool GetClickedNo();
	void SetClickedInit();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEWCLOSE };
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
