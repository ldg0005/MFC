
#include "pch.h"
#include "framework.h"
#include "MDIPainter.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "MDIPainterDoc.h"
#include "MDIPainterView.h"
#include "ViewClose.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CChildFrame
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
	ON_COMMAND(ID_VIEW_CLOSE, &CChildFrame::OnViewClose)
END_MESSAGE_MAP()
// CChildFrame 생성/소멸
CChildFrame::CChildFrame() noexcept
{
}
CChildFrame::~CChildFrame()
{
}
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}
// CChildFrame 진단
#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}
void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG
// CChildFrame 메시지 처리기
int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CString strCurTitle, strNum;

	if(pMain->m_bFormViewTitle)
	{
		strCurTitle = _T("Information");
		pMain->m_bFormViewTitle = false;
		pMain->ShowControlBar(&pMain->m_wndToolBarChild, FALSE, FALSE);
		pMain->ShowControlBar(&pMain->m_wndToolBarMain, TRUE, FALSE);
	}
	else
	{
		if(!strCurTitle.LoadString(AFX_IDS_UNTITLED))
			strCurTitle = _T("제목 없음");

		strNum.Format(_T("%d"), ++pMain->m_i32ChildTitleCnt);
		pMain->m_i32ChildCnt++;
		pMain->ShowControlBar(&pMain->m_wndToolBarChild, TRUE, FALSE);
		pMain->ShowControlBar(&pMain->m_wndToolBarMain, FALSE, FALSE);
	}

	SetWindowTextW(strCurTitle + strNum);
	UpdateWindow();

	return 0;
}
bool CChildFrame::CloseProc()
{
	CMDIPainterView* pView = (CMDIPainterView*)GetActiveView();
	ViewClose viewcloseTmp;
	bool bReturn = true;

	do
	{
		if(pView->m_bViewChanged)
		{
			viewcloseTmp.DoModal();

			if(viewcloseTmp.GetClickedYes())
			{
				pView->OnFileSave();

				if(pView->m_bViewChanged)
				{
					bReturn = false;
					break;
				}
			}
			else if(!viewcloseTmp.GetClickedYes() && !viewcloseTmp.GetClickedNo())
			{
				bReturn = false;
				break;
			}
		}
	}
	while(false);

	viewcloseTmp.SetClickedInit();

	return bReturn;
}
void CChildFrame::OnClose()
{
	if(CloseProc())
		OnExit();
}
void CChildFrame::OnExit()
{
	CMDIChildWnd::OnClose();
}
void CChildFrame::OnViewClose()
{
	if(CloseProc())
		OnExit();
}
void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMDIPainterView* pView = (CMDIPainterView*)GetActiveView();

	if(pActivateWnd == this)
	{
		pMain->ShowControlBar(&pMain->m_wndToolBarChild, TRUE, FALSE);
		pMain->ShowControlBar(&pMain->m_wndToolBarMain, FALSE, FALSE);
		pMain->m_comboboxLineThick.SetCurSel(pView->m_i32PxCost);
		pMain->m_comboboxColor.SetCurSel(pView->m_i32ColorCost);
		pMain->UpdateWindow();
	}
	
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}