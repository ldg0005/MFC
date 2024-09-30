#pragma once

#include "MainFrm.h"
#include "ChildFrm.h"
#include "MDIPainterDoc.h"
#include "MDIPainterView.h"
#include "FileClose.h"
#include "ViewClose.h"
#include "FormViewView.h"


class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// 특성입니다.
public:
// 작업입니다.
public:
	uint32_t GetCrc(uint8_t* pU8Buffer, uint32_t u32NByte);
	
	uint32_t m_arrU32CrcTable[256];
	CComboBox m_comboboxLineThick, m_comboboxColor;
	CToolBar m_wndToolBarChild, m_wndToolBarMain;
	int32_t m_i32ChildCnt = 0, m_i32ChildTitleCnt = 0;
	bool m_bFormView = false, m_bFormViewTitle = false;
	
	FormViewView* m_pFormView = nullptr;
	std::vector<POSITION> m_vctView;

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar        m_wndStatusBar;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnMainFileOpen();
	afx_msg void OnClose();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnToolbarDropDown(NMHDR* pNmhdr, LRESULT* pLr);
	afx_msg void OnUpdateFormviewNew(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};


