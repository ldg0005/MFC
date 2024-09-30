#pragma once

class FormViewView : public CFormView
{
	DECLARE_DYNCREATE(FormViewView)

protected:
	FormViewView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~FormViewView();
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FormView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void PushInfor(CMDIPainterView* pView);
	void UpdateInfor(CMDIPainterView* pSender);
	void ArrangeInfor();
	void MakeInfor();
	

	CString m_strFilename, m_strX, m_strY, m_strScale, m_strEdit;
	int32_t m_i32Right = 1175, m_i32Bottom = 402, m_i32WindowSizeKey = 2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
