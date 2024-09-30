#include "pch.h"

#include "MDIPainter.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "MDIPainterDoc.h"
#include "FormViewView.h"
#include "MDIPainterView.h"

IMPLEMENT_DYNCREATE(FormViewView, CFormView)
FormViewView::FormViewView()
	: CFormView(IDD_FormView)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	pMain->m_pFormView = this;
}
FormViewView::~FormViewView()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	pMain->m_bFormView = false;
	pMain->m_pFormView = nullptr;
}
void FormViewView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FormViewView, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

#ifdef _DEBUG
void FormViewView::AssertValid() const
{
	CFormView::AssertValid();
}
#ifndef _WIN32_WCE
void FormViewView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void FormViewView::UpdateInfor(CMDIPainterView* pSender)
{
	CString strTmp;

	m_strFilename = pSender->m_strTitle;
	m_strX.Format(_T("%f"), pSender->m_ptOffset.f64X);
	m_strY.Format(_T("%f"), pSender->m_ptOffset.f64Y);
	m_strScale.Format(_T("%f"), pSender->m_f64ZoomRate);

	if(pSender->m_bViewFirstChanged)
		m_strEdit = _T("YES");
	else
		m_strEdit = _T("NO");

	strTmp = pSender->m_pListView->GetItemText(1, 2);

	if(strTmp != m_strFilename)
		pSender->m_pListView->SetItemText(1, 2, m_strFilename);

	strTmp = pSender->m_pListView->GetItemText(2, 2);
	
	if(strTmp != m_strX)
		pSender->m_pListView->SetItemText(2, 2, m_strX);

	strTmp = pSender->m_pListView->GetItemText(3, 2);
	
	if(strTmp != m_strY)
		pSender->m_pListView->SetItemText(3, 2, m_strY);

	strTmp = pSender->m_pListView->GetItemText(4, 2);
	
	if(strTmp != m_strScale)
		pSender->m_pListView->SetItemText(4, 2, m_strScale);

	strTmp = pSender->m_pListView->GetItemText(5, 2);
	
	if(strTmp != m_strEdit)
		pSender->m_pListView->SetItemText(5, 2, m_strEdit);

	UpdateWindow();
}
void FormViewView::MakeInfor()
{
	CString strItem[6] = {_T("Item"), _T("Filename"),_T("Offset X"), _T("Offset Y"),_T("Scale"),_T("IsEdited") };
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CRect rtCur(0,0,0,0);

	for(int32_t i = 0; i < (int32_t)pMain->m_vctView.size(); i++)
	{
		CMDIPainterView* pView = (CMDIPainterView*)pMain->m_vctView[i];

		pView->m_pListView->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_NOCOLUMNHEADER, rtCur, this, NULL);
		pView->m_pListView->SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		pView->m_pListView->InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
		pView->m_pListView->InsertColumn(1, _T(""), LVCFMT_CENTER, 80, -1);
		pView->m_pListView->InsertColumn(2, _T(""), LVCFMT_CENTER, 170, -1);
		pView->m_pListView->ShowScrollBar(SB_VERT, 0);
		pView->m_pListView->ShowScrollBar(SB_HORZ, 0);
		
		for(int32_t j = 0; j < 6; j++)
		{
			pView->m_pListView->InsertItem(j, _T(""));
			pView->m_pListView->SetItemText(j, 1, strItem[j]);
		}

		pView->m_pListView->SetItemText(0, 2, _T("Value"));
		UpdateInfor(pView);
	}
}
void FormViewView::ArrangeInfor()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CRect rtCur;
	LONG i32OffsetH = GetScrollPos(SB_VERT);
	LONG i32OffsetW = GetScrollPos(SB_HORZ);
	LONG i32W = 0, i32H = 0;
	CSize szSize(0,0);

	do
	{
		for(int32_t i = 0; i < (int32_t)pMain->m_vctView.size(); i++)
		{
			CMDIPainterView* pView = (CMDIPainterView*)pMain->m_vctView[i];

			rtCur.left = 20 + (i32W * 270) - i32OffsetW;
			rtCur.top = 20 + (i32H * 134) - i32OffsetH;
			rtCur.right = rtCur.left + 250;
			rtCur.bottom = rtCur.top + 114;
			i32W++;

			if(i != 0 && rtCur.right > m_i32Right)
			{
				i32W = 1;
				i32H++;
				rtCur.left = 20 - i32OffsetW;
				rtCur.top = 20 + (i32H * 134) - i32OffsetH;
				rtCur.right = rtCur.left + 250;
				rtCur.bottom = rtCur.top + 114;
			}

			pView->m_pListView->MoveWindow(rtCur, true);
		}

		if(m_i32Right <= rtCur.right + i32OffsetW)
		{
			if((rtCur.bottom + i32OffsetH <= m_i32Bottom) && (rtCur.bottom + i32OffsetH + 17 >= m_i32Bottom))
				break;
			else
				szSize.cy = rtCur.bottom + i32OffsetH;
		}
		else
			szSize.cy = rtCur.bottom + i32OffsetH;

		if(m_i32Bottom <= rtCur.bottom + i32OffsetH)
		{
			if((rtCur.right + i32OffsetW <= m_i32Right) && (rtCur.right + i32OffsetW + 17 >= m_i32Right))
				break;
			else
				szSize.cx = min(270, rtCur.right + i32OffsetW);
		}
		else
			szSize.cx = min(270, rtCur.right + i32OffsetW);

		SetScrollSizes(MM_TEXT, szSize);
		UpdateWindow();
	}
	while(false);
	
}
void FormViewView::PushInfor(CMDIPainterView* pView)
{
	CString strItem[6] = { _T("Item"), _T("Filename"),_T("Offset X"), _T("Offset Y"),_T("Scale"),_T("IsEdited") };
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CRect rtCur(0, 0, 0, 0);

	pView->m_pListView->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_NOCOLUMNHEADER, rtCur, this, NULL);
	pView->m_pListView->SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	pView->m_pListView->InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	pView->m_pListView->InsertColumn(1, _T(""), LVCFMT_CENTER, 80, -1);
	pView->m_pListView->InsertColumn(2, _T(""), LVCFMT_CENTER, 170, -1);
	pView->m_pListView->ShowScrollBar(SB_VERT, 0);
	pView->m_pListView->ShowScrollBar(SB_HORZ, 0);

	for(int32_t j = 0; j < 6; j++)
	{
		pView->m_pListView->InsertItem(j, _T(""));
		pView->m_pListView->SetItemText(j, 1, strItem[j]);
	}

	pView->m_pListView->SetItemText(0, 2, _T("Value"));
	UpdateInfor(pView);
	ArrangeInfor();
}
int FormViewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetScrollSizes(MM_TEXT, CSize(0,0));
	MakeInfor();
	ArrangeInfor();

	return 0;
}
void FormViewView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	if(pActivateView == this)
	{
		pMain->ShowControlBar(&pMain->m_wndToolBarChild, FALSE, FALSE);
		pMain->ShowControlBar(&pMain->m_wndToolBarMain, TRUE, FALSE);
		pMain->UpdateWindow();
	}
}
void FormViewView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if(m_i32WindowSizeKey < 1)
	{
		m_i32Right = cx;
		m_i32Bottom = cy;
		ArrangeInfor();
	}
	else
		m_i32WindowSizeKey--;
}
