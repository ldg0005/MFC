
#pragma once
#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif
#include "resource.h"       // 주 기호입니다.



class CMDIPainterApp : public CWinApp
{
public:
	CMDIPainterApp() noexcept;
// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFormViewNew();
	DECLARE_MESSAGE_MAP()
};

extern CMDIPainterApp theApp;
