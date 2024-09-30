#pragma warning(disable: 26454)
#include "pch.h"
#include "framework.h"
#include "MDIPainter.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "MDIPainterDoc.h"
#include "MDIPainterView.h"
#include "FileClose.h"
#include "ViewClose.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_MAINFILE_OPEN, &CMainFrame::OnMainFileOpen)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, &CMainFrame::OnToolbarDropDown)
	ON_UPDATE_COMMAND_UI(ID_FORMVIEW_NEW, &CMainFrame::OnUpdateFormviewNew)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
// CMainFrame 생성/소멸
CMainFrame::CMainFrame() noexcept
{
	for(uint32_t i = 0; i < 256; i++)
	{
		uint32_t u32Remainder = i;

		for(uint32_t j = 0; j < 8; j++)
		{
			if(u32Remainder & 1)
			{
				u32Remainder >>= 1;
				u32Remainder ^= 0xEDB88320;
			}
			else
				u32Remainder >>= 1;
		}

		m_arrU32CrcTable[i] = u32Remainder;
	}
}
CMainFrame::~CMainFrame()
{
}
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CMDIFrameWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}
// CMainFrame 진단
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}
void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG
// CMainFrame 메시지 처리기

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	CRect rtCreate;
	int32_t i32Index, i32Return=0;
	DWORD wordStyle;

	do
	{
		if(!m_wndToolBarMain.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) || !m_wndToolBarMain.LoadToolBar(IDR_MAINFRAME))
		{
			TRACE0("도구 모음을 만들지 못했습니다.\n");
			i32Return = -1;
			break;
		}

		m_wndToolBarMain.EnableDocking(CBRS_ALIGN_ANY);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndToolBarMain);

		m_wndToolBarMain.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
		wordStyle = m_wndToolBarMain.GetButtonStyle(m_wndToolBarMain.CommandToIndex(ID_FILE_NEW));
		wordStyle |= TBSTYLE_DROPDOWN;
		m_wndToolBarMain.SetButtonStyle(m_wndToolBarMain.CommandToIndex(ID_FILE_NEW), wordStyle);
		m_wndToolBarMain.SetButtonInfo(m_wndToolBarMain.CommandToIndex(ID_MYSEPARATOR), ID_MYSEPARATOR, TBBS_SEPARATOR, 6);

		if(!m_wndToolBarChild.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) || !m_wndToolBarChild.LoadToolBar(IDR_MDIPainterTYPE))
		{
			TRACE0("도구 모음을 만들지 못했습니다.\n");
			i32Return = -1;
			break;
		}

		m_wndToolBarChild.EnableDocking(CBRS_ALIGN_ANY);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndToolBarChild);

		m_wndToolBarChild.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
		wordStyle = m_wndToolBarChild.GetButtonStyle(m_wndToolBarChild.CommandToIndex(ID_FILE_NEW));
		wordStyle |= TBSTYLE_DROPDOWN;
		m_wndToolBarChild.SetButtonStyle(m_wndToolBarChild.CommandToIndex(ID_FILE_NEW), wordStyle);
		m_wndToolBarChild.SetButtonInfo(m_wndToolBarChild.CommandToIndex(ID_MYSEPARATOR), ID_MYSEPARATOR, TBBS_SEPARATOR, 6);
		m_wndToolBarChild.SetButtonInfo(m_wndToolBarChild.CommandToIndex(ID_MYSEPARATOR2), ID_MYSEPARATOR2, TBBS_SEPARATOR, 14);


		i32Index = m_wndToolBarChild.CommandToIndex(ID_LINE_THICK);
		m_wndToolBarChild.SetButtonInfo(i32Index, ID_LINE_THICK, TBBS_SEPARATOR, 60);
		m_wndToolBarChild.GetToolBarCtrl().GetItemRect(i32Index, &rtCreate);


		if(!m_comboboxLineThick.Create(CBS_DROPDOWNLIST | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL, rtCreate, &m_wndToolBarChild, ID_LINE_THICK))
		{
			TRACE(_T("Failed to create combo-box\n"));
			i32Return = -1;
			break;
		}

		m_comboboxLineThick.AddString(_T("1 px"));
		m_comboboxLineThick.AddString(_T("3 px"));
		m_comboboxLineThick.AddString(_T("5 px"));
		m_comboboxLineThick.SetCurSel(0);

		i32Index = m_wndToolBarChild.GetToolBarCtrl().CommandToIndex(ID_COLOR);
		m_wndToolBarChild.SetButtonInfo(i32Index, ID_COLOR, TBBS_SEPARATOR, 110);
		m_wndToolBarChild.GetToolBarCtrl().GetItemRect(i32Index, &rtCreate);

		if(!m_comboboxColor.Create(CBS_DROPDOWNLIST | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL, rtCreate, &m_wndToolBarChild, ID_COLOR))
		{
			TRACE(_T("Failed to create combo-box\n"));
			i32Return = -1;
			break;
		}

		m_comboboxColor.AddString(_T("BLACK"));
		m_comboboxColor.AddString(_T("LIGHTRED"));
		m_comboboxColor.AddString(_T("LIME"));
		m_comboboxColor.AddString(_T("BLUE"));
		m_comboboxColor.AddString(_T("User defined"));
		m_comboboxColor.SetCurSel(0);

		UpdateWindow();
	}
	while(false);

	return i32Return;
}

void CMainFrame::OnToolbarDropDown(NMHDR* pNmhdr, LRESULT* pLresult)
{
	CWnd* pWnd = nullptr;
	LPNMTOOLBAR pNmtb = reinterpret_cast<LPNMTOOLBAR>(pNmhdr);
	CRect rtToolbar;
	CMenu menuPop;
	
	do
	{
		if(pNmtb->iItem == ID_FILE_NEW)
			pWnd = &m_wndToolBarMain;
		else
			break;

		pWnd->SendMessage(TB_GETRECT, pNmtb->iItem, (LPARAM)&rtToolbar);
		pWnd->ClientToScreen(&rtToolbar);

		menuPop.CreatePopupMenu();
		menuPop.AppendMenuW(MF_STRING, ID_FILE_NEW, _T("Paint View"));

		if(!m_bFormView)
			menuPop.AppendMenuW(MF_STRING, ID_FORMVIEW_NEW, _T("Information View"));
		else
			menuPop.AppendMenuW(MF_STRING | MF_GRAYED, ID_FORMVIEW_NEW, _T("Information View"));

		menuPop.TrackPopupMenu(TPM_LEFTALIGN, rtToolbar.left, rtToolbar.bottom, this);
	}
	while(false);

	*pLresult = 0;
}
void CMainFrame::OnClose()
{
	FileClose filecloseTmp;
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = nullptr;

	do
	{
		filecloseTmp.DoModal();

		if(filecloseTmp.m_bClickedYes)
		{
			filecloseTmp.m_bClickedYes = false;

			while(m_i32ChildCnt>=0 || pMain->m_bFormView)
			{
				pChild = (CChildFrame*)GetActiveFrame();

				if(pChild == (CChildFrame*)this)
				{
					CMDIFrameWnd::OnClose();
					break;
				}

				if(pChild->CloseProc())
					pChild->OnExit();
				else
					break;
			}
		}
	}
	while(false);
}
void CMainFrame::OnFileNew()
{
	CMDIPainterApp* pApp = (CMDIPainterApp*)AfxGetApp();

	pApp->OnFileNew();
}
void CMainFrame::OnMainFileOpen()
{
	OnFileOpen();
}
void CMainFrame::OnFileOpen()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMDIPainterApp* pApp = (CMDIPainterApp*)AfxGetApp();
	CChildFrame* pChild = (CChildFrame*)GetActiveFrame();
	CMDIPainterView* pView = nullptr;
	CString strCurTitle, strPath;
	uint32_t u32VertexSize = 0, u32VertexGroupCount = 0, u32NthVertexCount = 0, u32Size = 19;
	uint8_t u8Header[11] = { 0, }, u8ForHeaderCmp[12] = "FourthLogic", *pU8Buffer = nullptr;
	FILE* pFileSave = nullptr;
	std::vector<SPoint> vctTmp;
	std::vector<std::vector<SPoint> > vctBuffer;
	bool bChkCorrupted = false;

	do
	{
		CFileDialog filedialogDlg(TRUE, _T("flp"), NULL, OFN_HIDEREADONLY, _T("flp Files (*.flp) |*.flp|"), NULL);

		if(filedialogDlg.DoModal() == IDOK)
		{
			uint32_t u32TmpSize = 0, u32Crc = 0;

			strCurTitle = filedialogDlg.GetFileTitle();
			strPath = filedialogDlg.GetPathName();

			if(_wfopen_s(&pFileSave, (strPath), _T("r+b")) == 0)
			{
				if(pFileSave == nullptr)
					break;

				fseek(pFileSave, 0, SEEK_END);
				u32TmpSize = ftell(pFileSave);
				pU8Buffer = new uint8_t[u32TmpSize - 4];
				fseek(pFileSave, 0, SEEK_SET);
				fread(pU8Buffer, 1, u32TmpSize - 4, pFileSave);
				fread(&u32Crc, 1, 4, pFileSave);

				if(u32Crc != GetCrc(pU8Buffer, u32TmpSize - 4))
				{
					bChkCorrupted = true;
					break;
				}

				fseek(pFileSave, 0, SEEK_SET);
				fread(&u8Header, 1, 11, pFileSave);
				fread(&u32VertexSize, 1, 4, pFileSave);
				fread(&u32VertexGroupCount, 1, 4, pFileSave);

				for(int32_t i = 0; i < 11; i++)
				{
					if(u8Header[i] != u8ForHeaderCmp[i])
					{
						bChkCorrupted = true;
						break;
					}
				}

				if(bChkCorrupted || u32VertexSize != 24)
				{
					bChkCorrupted = true;
					break;
				}

				for(uint32_t i = 0; i < u32VertexGroupCount; i++)
				{
					fread(&u32NthVertexCount, 1, sizeof(u32NthVertexCount), pFileSave);
					vctBuffer.push_back(vctTmp);

					if((u32Size + (u32NthVertexCount * 24)) > (u32TmpSize - 4))
					{
						bChkCorrupted = true;
						break;
					}

					u32Size += 4;

					for(uint32_t j = 0; j < u32NthVertexCount; j++)
					{
						double f64X, f64Y;
						int32_t i32Px;
						COLORREF colorrefColor;

						u32Size += u32VertexSize;
						fread(&f64X, 1, sizeof(f64X), pFileSave);
						fread(&f64Y, 1, sizeof(f64Y), pFileSave);
						fread(&i32Px, 1, sizeof(i32Px), pFileSave);
						fread(&colorrefColor, 1, sizeof(colorrefColor), pFileSave);

						SPoint ptIn(f64X, f64Y, i32Px, colorrefColor);

						vctBuffer[i].push_back(ptIn);
					}
				}

				if(bChkCorrupted || u32Size != u32TmpSize - 4)
				{
					bChkCorrupted = true;
					break;
				}

				fclose(pFileSave);
			}
			else
			{
				MessageBox(_T("Failed to open the file."));
				break;
			}

			pApp->OnFileNew();
			pChild = (CChildFrame*)GetActiveFrame();
			pView = (CMDIPainterView*)pChild->GetActiveView();
			pView->m_strPath = strPath;
			pView->m_vctLine = vctBuffer;
			pView->m_strTitle = strCurTitle;
			pView->m_bViewFirstChanged = pView->m_bViewChanged = false;
			pView->m_bSelectedFile = true;
			pView->m_f64ZoomRate = 1.0;
			pView->m_ptOffset.f64X = pView->m_ptOffset.f64Y = 0.0;
			pChild->SetWindowTextW(strCurTitle);
			pChild->UpdateWindow();

			if(pMain->m_pFormView)
				pMain->m_pFormView->UpdateInfor(pView);
		}
	}
	while(false);

	if(bChkCorrupted)
	{
		if(pFileSave != nullptr)
			fclose(pFileSave);

		MessageBox(_T("File is corrupted."));
	}

	if(pU8Buffer)
	{
		delete[] pU8Buffer;
		pU8Buffer= nullptr;
	}

	pView->Invalidate(false);
}
void CMainFrame::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menuPop,menuNew;

	menuNew.CreatePopupMenu();
	menuNew.AppendMenuW(MF_STRING, ID_FILE_NEW, _T("Paint View"));

	if(!m_bFormView)
		menuNew.AppendMenuW(MF_STRING, ID_FORMVIEW_NEW, _T("Information View"));
	else
		menuNew.AppendMenuW(MF_STRING | MF_GRAYED, ID_FORMVIEW_NEW, _T("Information View"));

	menuPop.CreatePopupMenu();
	menuPop.AppendMenuW(MF_POPUP | MF_STRING, (UINT_PTR)menuNew.GetSafeHmenu(), _T("새로 만들기(&N)"));
	menuPop.AppendMenuW(MF_SEPARATOR);
	menuPop.AppendMenuW(MF_STRING, ID_MAINFILE_OPEN, _T("열기(&O)"));
	menuPop.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}
uint32_t CMainFrame::GetCrc(uint8_t* pU8Buffer, uint32_t u32NByte)
{
	uint32_t u32Remainder = 0;
	uint8_t u8Idx;

	u32Remainder = ~u32Remainder;

	for(uint32_t i = 0; i < u32NByte; i++)
	{
		u8Idx = pU8Buffer[i];
		u32Remainder = (u32Remainder >> 8) ^ m_arrU32CrcTable[(u32Remainder & 0xFF) ^ u8Idx];
	}

	return ~u32Remainder;
}
void CMainFrame::OnUpdateFormviewNew(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_bFormView);
}
