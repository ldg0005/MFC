
#include "pch.h"
#include "framework.h"

#ifndef SHARED_HANDLERS
#include "MDIPainter.h"
#endif

#include "MainFrm.h"
#include "ChildFrm.h"
#include "MDIPainterDoc.h"
#include "MDIPainterView.h"
#include "NewCanvas.h"
#include "ViewClose.h"
#include "FileOpen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
IMPLEMENT_DYNCREATE(CMDIPainterView, CView)
BEGIN_MESSAGE_MAP(CMDIPainterView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_NEW, &CMDIPainterView::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CMDIPainterView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CMDIPainterView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMDIPainterView::OnFileSaveAs)
	ON_COMMAND(ID_VIEW_CLOSE, &CMDIPainterView::OnViewClose)
	ON_COMMAND(ID_THICK_PX1, &CMDIPainterView::OnThickPx1)
	ON_COMMAND(ID_THICK_PX3, &CMDIPainterView::OnThickPx3)
	ON_COMMAND(ID_THICK_PX5, &CMDIPainterView::OnThickPx5)
	ON_COMMAND(ID_COLOR_BLACK, &CMDIPainterView::OnColorBlack)
	ON_COMMAND(ID_COLOR_RED, &CMDIPainterView::OnColorRed)
	ON_COMMAND(ID_COLOR_LIME, &CMDIPainterView::OnColorLime)
	ON_COMMAND(ID_COLOR_BLUE, &CMDIPainterView::OnColorBlue)
	ON_COMMAND(ID_COLOR_USERDEF, &CMDIPainterView::OnColorUserdef)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CMDIPainterView::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_THICK_PX1, &CMDIPainterView::OnUpdateThickPx1)
	ON_UPDATE_COMMAND_UI(ID_THICK_PX3, &CMDIPainterView::OnUpdateThickPx3)
	ON_UPDATE_COMMAND_UI(ID_THICK_PX5, &CMDIPainterView::OnUpdateThickPx5)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLACK, &CMDIPainterView::OnUpdateColorBlack)
	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, &CMDIPainterView::OnUpdateColorRed)
	ON_UPDATE_COMMAND_UI(ID_COLOR_LIME, &CMDIPainterView::OnUpdateColorLime)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLUE, &CMDIPainterView::OnUpdateColorBlue)
	ON_UPDATE_COMMAND_UI(ID_COLOR_USERDEF, &CMDIPainterView::OnUpdateColorUserdef)
	ON_COMMAND(Px1, &CMDIPainterView::OnPx1)
	ON_COMMAND(Px3, &CMDIPainterView::OnPx3)
	ON_COMMAND(Px5, &CMDIPainterView::OnPx5)
	ON_COMMAND(BLACK, &CMDIPainterView::OnBlack)
	ON_COMMAND(RED, &CMDIPainterView::OnRed)
	ON_COMMAND(LIME, &CMDIPainterView::OnLime)
	ON_COMMAND(BLUE, &CMDIPainterView::OnBlue)
	ON_WM_CONTEXTMENU()
	ON_CBN_SELENDOK(ID_LINE_THICK, &OnBnClickedLineThick)
	ON_CBN_SELENDOK(ID_COLOR, &OnBnClickedColor)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FORMVIEW_NEW, &CMDIPainterView::OnFormViewNew)
END_MESSAGE_MAP()

CMDIPainterView::CMDIPainterView() noexcept
{
	CRect rtCur;
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	CString strNum;

	m_pListView = new MyListCtrl;
	pMain->m_vctView.push_back((POSITION)this); 
	
	if(!m_strTitle.LoadString(AFX_IDS_UNTITLED))
		m_strTitle = _T("제목 없음");

	strNum.Format(_T("%d"), pMain->m_i32ChildTitleCnt + 1);
	m_strTitle += strNum;
	
	if(pMain->m_pFormView)
		pMain->m_pFormView->PushInfor(this);

	m_arrPenMyColor[0][0].CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	m_arrPenMyColor[1][0].CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	m_arrPenMyColor[2][0].CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	m_arrPenMyColor[0][1].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPenMyColor[1][1].CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	m_arrPenMyColor[2][1].CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	m_arrPenMyColor[0][2].CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPenMyColor[1][2].CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	m_arrPenMyColor[2][2].CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	m_arrPenMyColor[0][3].CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_arrPenMyColor[1][3].CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	m_arrPenMyColor[2][3].CreatePen(PS_SOLID, 5, RGB(0, 0, 255));

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
CMDIPainterView::~CMDIPainterView()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	int32_t i32Idx = 0;

	if(!--pMain->m_i32ChildCnt)
	{
		pMain->ShowControlBar(&pMain->m_wndToolBarChild, FALSE, FALSE);
		pMain->ShowControlBar(&pMain->m_wndToolBarMain, TRUE, FALSE);
	}

	for(i32Idx = 0; i32Idx < (int32_t)pMain->m_vctView.size(); i32Idx++)
	{
		if((CMDIPainterView*)pMain->m_vctView[i32Idx] == this)
			break;
	}

	std::vector<POSITION>::iterator iter = pMain->m_vctView.begin();

	iter += i32Idx;
	pMain->m_vctView.erase(iter);

	if(pMain->m_pFormView)
		pMain->m_pFormView->ArrangeInfor();

	if(m_pListView)
	{
		delete m_pListView;
		m_pListView = nullptr;
	}
}
BOOL CMDIPainterView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}
#ifdef _DEBUG
void CMDIPainterView::AssertValid() const
{
	CView::AssertValid();
}
void CMDIPainterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CMDIPainterDoc* CMDIPainterView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDIPainterDoc)));
	return (CMDIPainterDoc*)m_pDocument;
}
#endif //_DEBUG


void CMDIPainterView::OnDraw(CDC* pDC)
{
	CMDIPainterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;

	CRect rtMem;
	CDC dcMem;
	CBitmap bmpMem;
	int32_t i32PrevPx = 0;
	COLORREF colorrefPrev = RGB(0, 0, 0);
	CPen penCur, * pPenOld = nullptr;

	GetClientRect(&rtMem);
	dcMem.CreateCompatibleDC(pDC);
	bmpMem.CreateCompatibleBitmap(pDC, rtMem.Width(), rtMem.Height());
	dcMem.SelectObject(&bmpMem);
	dcMem.PatBlt(0, 0, rtMem.right, rtMem.bottom, WHITENESS);

	for(int32_t i = 0; i < (int32_t)m_vctLine.size(); i++)
	{
		for(int32_t j = 0, k = 1; k < (int32_t)m_vctLine[i].size() - 1; j++, k++)
		{
			SPoint ptFirstStart, ptFirstEnd, ptSecondStart, ptSecondEnd;

			if(m_vctLine[i][j].i32Px != i32PrevPx || m_vctLine[i][j].colorrefColor != colorrefPrev)
			{
				if(m_vctLine[i][j].colorrefColor != RGB(255, 0, 0) && m_vctLine[i][j].colorrefColor != RGB(0, 255, 0) && m_vctLine[i][j].colorrefColor != RGB(0, 0, 255) && m_vctLine[i][j].colorrefColor != RGB(0, 0, 0))
				{
					penCur.DeleteObject();

					if(m_vctLine[i][j].i32Px==0)
						penCur.CreatePen(PS_SOLID, 1, m_vctLine[i][j].colorrefColor);
					else if(m_vctLine[i][j].i32Px == 1)
						penCur.CreatePen(PS_SOLID, 3, m_vctLine[i][j].colorrefColor);
					else
						penCur.CreatePen(PS_SOLID, 5, m_vctLine[i][j].colorrefColor);
					
					pPenOld = dcMem.SelectObject(&penCur);
				}
				else
				{
					if(m_vctLine[i][j].colorrefColor == RGB(0, 0, 0))
						pPenOld = dcMem.SelectObject(&m_arrPenMyColor[m_vctLine[i][j].i32Px][0]);
					else if(m_vctLine[i][j].colorrefColor == RGB(255, 0, 0))
						pPenOld = dcMem.SelectObject(&m_arrPenMyColor[m_vctLine[i][j].i32Px][1]);
					else if(m_vctLine[i][j].colorrefColor == RGB(0, 255, 0))
						pPenOld = dcMem.SelectObject(&m_arrPenMyColor[m_vctLine[i][j].i32Px][2]);
					else if(m_vctLine[i][j].colorrefColor == RGB(0, 0, 255))
						pPenOld = dcMem.SelectObject(&m_arrPenMyColor[m_vctLine[i][j].i32Px][3]);
				}

				i32PrevPx = m_vctLine[i][j].i32Px, colorrefPrev = m_vctLine[i][j].colorrefColor;
			}

			ptFirstStart = RealToScreen(m_vctLine[i][j]);
			ptFirstEnd = RealToScreen(m_vctLine[i][k]);

			if(ChkInside(ptFirstStart, (double)rtMem.right, (double)rtMem.bottom) && ChkInside(ptFirstEnd, (double)rtMem.right, (double)rtMem.bottom))
			{
				if(((int32_t)ptFirstStart.f64X == (int32_t)ptFirstEnd.f64X) && ((int32_t)ptFirstStart.f64Y == (int32_t)ptFirstEnd.f64Y))
					dcMem.SetPixel(ptFirstStart, m_vctLine[i][j].colorrefColor);
				else
				{
					dcMem.MoveTo(ptFirstStart);
					dcMem.LineTo(ptFirstEnd);
				}
			}
			else
			{
				double f64Right = (double)rtMem.right + 50.0, f64Bottom = (double)rtMem.bottom + 50.0;
				double arrF64X[4] = { -50.0,f64Right,f64Right,-50.0 }, arrF64Y[4] = { -50.0,-50.0,f64Bottom,f64Bottom };
				bool bFlag = false;
				SPoint* pPtTmp = new SPoint;

				for(int32_t k = 0; k < 4; k++)
				{
					ptSecondStart.f64X = arrF64X[k], ptSecondStart.f64Y = arrF64Y[k];
					ptSecondEnd.f64X = arrF64X[(k + 1) % 4], ptSecondEnd.f64Y = arrF64Y[(k + 1) % 4];

					if(Ccw(ptFirstStart.f64X, ptFirstStart.f64Y, ptFirstEnd.f64X, ptFirstEnd.f64Y, ptSecondStart.f64X, ptSecondStart.f64Y)
					   * Ccw(ptFirstStart.f64X, ptFirstStart.f64Y, ptFirstEnd.f64X, ptFirstEnd.f64Y, ptSecondEnd.f64X, ptSecondEnd.f64Y) < 0
					   && Ccw(ptSecondStart.f64X, ptSecondStart.f64Y, ptSecondEnd.f64X, ptSecondEnd.f64Y, ptFirstStart.f64X, ptFirstStart.f64Y)
					   * Ccw(ptSecondStart.f64X, ptSecondStart.f64Y, ptSecondEnd.f64X, ptSecondEnd.f64Y, ptFirstEnd.f64X, ptFirstEnd.f64Y) < 0)
					{
						GetCrossPoint(ptFirstStart, ptFirstEnd, ptSecondStart, ptSecondEnd, pPtTmp);

						if(ChkInside(ptFirstStart, (double)rtMem.right, (double)rtMem.bottom) && !ChkInside(ptFirstEnd, (double)rtMem.right, (double)rtMem.bottom))
						{
							dcMem.MoveTo(ptFirstStart);
							dcMem.LineTo(*pPtTmp);
							break;
						}
						else if(!ChkInside(ptFirstStart, (double)rtMem.right, (double)rtMem.bottom) && ChkInside(ptFirstEnd, (double)rtMem.right, (double)rtMem.bottom))
						{
							dcMem.MoveTo(ptFirstEnd);
							dcMem.LineTo(*pPtTmp);
							break;
						}
						else
						{
							if(bFlag)
							{
								dcMem.LineTo(*pPtTmp);
								break;
							}
							else
							{
								dcMem.MoveTo(*pPtTmp);
								bFlag = true;
							}
						}
					}

				}

				if(pPtTmp != nullptr)
				{
					delete pPtTmp;
					pPtTmp = nullptr;
				}
			}
		}
	}

	
	pDC->BitBlt(0, 0, rtMem.right, rtMem.bottom, &dcMem, 0, 0, SRCCOPY);
	dcMem.DeleteDC();
}


SPoint CMDIPainterView::RealToScreen(SPoint ptIn)
{
	SPoint ptReturn = ptIn;

	ptReturn.f64X = (ptIn.f64X * m_f64ZoomRate) + m_ptOffset.f64X;
	ptReturn.f64Y = (ptIn.f64Y * m_f64ZoomRate) + m_ptOffset.f64Y;

	return ptReturn;
}
SPoint CMDIPainterView::ScreenToReal(SPoint ptIn)
{
	SPoint ptReturn = ptIn;

	ptReturn.f64X = (ptIn.f64X - m_ptOffset.f64X) / m_f64ZoomRate;
	ptReturn.f64Y = (ptIn.f64Y - m_ptOffset.f64Y) / m_f64ZoomRate;

	return ptReturn;
}
void CMDIPainterView::OnFileNew()
{
	CMDIPainterApp* pApp = (CMDIPainterApp*)AfxGetApp();

	pApp->OnFileNew();
}
void CMDIPainterView::OnFileOpen()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	CString strCurTitle;
	uint32_t u32VertexSize = 0, u32VertexGroupCount = 0, u32NthVertexCount = 0, u32Size = 19;
	uint8_t u8Header[11] = { 0, }, u8ForHeaderCmp[12] = "FourthLogic", *pU8Buffer = nullptr;
	FILE* pFileSave = nullptr;
	std::vector<SPoint> vctTmp;
	std::vector<std::vector<SPoint> > vctBuffer;
	bool bChkCorrupted = false;

	do
	{
		if(m_bViewFirstChanged)
		{
			FileOpen fileopenTmp;

			fileopenTmp.DoModal();

			if(fileopenTmp.GetClickedYes())
			{
				OnFileSave();

				if(m_bViewFirstChanged)
				{
					fileopenTmp.SetClickedInit();
					break;
				}
			}
			else if(!fileopenTmp.GetClickedYes() && !fileopenTmp.GetClickedNo())
			{
				fileopenTmp.SetClickedInit();
				break;
			}
		}

		CFileDialog filedialogDlg(TRUE, _T("flp"), NULL, OFN_HIDEREADONLY, _T("flp Files (*.flp) |*.flp|"), NULL);

		if(filedialogDlg.DoModal() == IDOK)
		{
			uint32_t u32TmpSize = 0, u32Crc = 0;

			strCurTitle = filedialogDlg.GetFileTitle();
			m_strPath = filedialogDlg.GetPathName();

			if(_wfopen_s(&pFileSave, (m_strPath), _T("r+b")) == 0)
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

			m_vctLine.clear();
			m_vctLine = vctBuffer;
			m_bViewFirstChanged = m_bViewChanged = false;
			m_bSelectedFile = true;
			m_f64ZoomRate = 1.0;
			m_ptOffset.f64X = m_ptOffset.f64Y = 0.0;
			pChild->SetWindowTextW(strCurTitle);
			pChild->UpdateWindow();
			m_strTitle = strCurTitle;

			if(pMain->m_pFormView)
				pMain->m_pFormView->UpdateInfor(this);
		}
	}
	while(false);

	if(pU8Buffer)
	{
		delete[] pU8Buffer;
		pU8Buffer = nullptr;
	}
	
	if(bChkCorrupted)
	{
		if(pFileSave != nullptr)
			fclose(pFileSave);
		
		MessageBox(_T("File is corrupted."));
	}

	Invalidate(false);
}
void CMDIPainterView::OnFileSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	CString strCurTitle;
	uint32_t u32VertexSize = sizeof(double) + sizeof(double) + sizeof(int32_t) + sizeof(COLORREF);
	uint32_t u32VertexGroupCount = (uint32_t)m_vctLine.size();
	uint32_t u32Size = 19, u32Crc = 0;
	uint8_t u8Header[] = "FourthLogic",*pU8Buffer = nullptr;
	FILE* pFileSave = nullptr;

	do
	{
		if(m_bSelectedFile)
		{
			if(_wfopen_s(&pFileSave, (m_strPath), _T("w+b")) == 0)
			{
				if(pFileSave == nullptr)
					break;
				
				fwrite(&u8Header, 1, 11, pFileSave);
				fwrite(&u32VertexSize, 1, sizeof(u32VertexSize), pFileSave);
				fwrite(&u32VertexGroupCount, 1, sizeof(u32VertexGroupCount), pFileSave);

				for(uint32_t i = 0; i < u32VertexGroupCount; i++)
				{
					uint32_t u32NthVertexCount = (uint32_t)m_vctLine[i].size();

					fwrite(&u32NthVertexCount, 1, sizeof(u32NthVertexCount), pFileSave);
					u32Size += 4;

					for(uint32_t j = 0; j < u32NthVertexCount; j++)
					{
						u32Size += u32VertexSize;
						fwrite(&m_vctLine[i][j].f64X, 1, sizeof(m_vctLine[i][j].f64X), pFileSave);
						fwrite(&m_vctLine[i][j].f64Y, 1, sizeof(m_vctLine[i][j].f64Y), pFileSave);
						fwrite(&m_vctLine[i][j].i32Px, 1, sizeof(m_vctLine[i][j].i32Px), pFileSave);
						fwrite(&m_vctLine[i][j].colorrefColor, 1, sizeof(m_vctLine[i][j].colorrefColor), pFileSave);
					}
				}

				fseek(pFileSave, 0, SEEK_SET);
				pU8Buffer = new uint8_t[u32Size];
				fread(pU8Buffer, 1, u32Size, pFileSave);
				u32Crc = GetCrc(pU8Buffer, u32Size);
				fwrite(&u32Crc, 1, sizeof(u32Crc), pFileSave);

				fclose(pFileSave);
			}
			else
			{
				MessageBox(_T("Failed to save the file."));
				break;
			}

			if(pU8Buffer)
			{
				delete[] pU8Buffer;
				pU8Buffer = nullptr;
			}

			pChild->GetWindowTextW(strCurTitle);

			if(strCurTitle[0] == '*')
			{
				strCurTitle.Delete(0, 1);
				pChild->SetWindowTextW(strCurTitle);
				m_strTitle = strCurTitle;

				if(pMain->m_pFormView)
					pMain->m_pFormView->UpdateInfor(this);
			}

			m_bViewFirstChanged = m_bViewChanged = false;
			pMain->m_wndToolBarChild.SetButtonInfo(3, ID_FILE_SAVE, TBBS_DISABLED, 3);
			pMain->UpdateWindow();

			if(pMain->m_pFormView)
				pMain->m_pFormView->UpdateInfor(this);
		}
		else
			OnFileSaveAs();
	}
	while(false);
}
void CMDIPainterView::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bViewChanged);
}
void CMDIPainterView::OnFileSaveAs()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	CString strCurTitle;
	uint32_t u32VertexSize = sizeof(double) + sizeof(double) + sizeof(int32_t) + sizeof(COLORREF);
	uint32_t u32VertexGroupCount = (uint32_t)m_vctLine.size();
	uint32_t u32Size = 19, u32Crc = 0;
	uint8_t u8Header[] = "FourthLogic", *pU8Buffer = nullptr;
	FILE* pFileSave = nullptr;
	CFileDialog filedialogDlg(FALSE, _T("flp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("flp Files (*.flp) |*.flp|"), NULL);

	do
	{
		if(filedialogDlg.DoModal() == IDOK)
		{
			strCurTitle = filedialogDlg.GetFileTitle();
			m_strPath = filedialogDlg.GetPathName();

			if(filedialogDlg.GetFileExt() != "flp")
			{
				strCurTitle += ".";
				strCurTitle += filedialogDlg.GetFileExt();
				m_strPath += ".flp";
			}

			if(_wfopen_s(&pFileSave, (m_strPath), _T("w+b")) == 0)
			{
				if(pFileSave == nullptr)
					break;

				fwrite(&u8Header, 1, 11, pFileSave);
				fwrite(&u32VertexSize, 1, sizeof(u32VertexSize), pFileSave);
				fwrite(&u32VertexGroupCount, 1, sizeof(u32VertexGroupCount), pFileSave);

				for(uint32_t i = 0; i < u32VertexGroupCount; i++)
				{
					uint32_t u32NthVertexCount = (uint32_t)m_vctLine[i].size();

					fwrite(&u32NthVertexCount, 1, sizeof(u32NthVertexCount), pFileSave);
					u32Size += 4;

					for(uint32_t j = 0; j < u32NthVertexCount; j++)
					{
						u32Size += u32VertexSize;
						fwrite(&m_vctLine[i][j].f64X, 1, sizeof(m_vctLine[i][j].f64X), pFileSave);
						fwrite(&m_vctLine[i][j].f64Y, 1, sizeof(m_vctLine[i][j].f64Y), pFileSave);
						fwrite(&m_vctLine[i][j].i32Px, 1, sizeof(m_vctLine[i][j].i32Px), pFileSave);
						fwrite(&m_vctLine[i][j].colorrefColor, 1, sizeof(m_vctLine[i][j].colorrefColor), pFileSave);
					}
				}

				fseek(pFileSave, 0, SEEK_SET);
				pU8Buffer = new uint8_t[u32Size];
				fread(pU8Buffer, 1, u32Size, pFileSave);
				u32Crc = GetCrc(pU8Buffer, u32Size);
				fwrite(&u32Crc, 1, sizeof(u32Crc), pFileSave);

				fclose(pFileSave);
			}
			else
			{
				MessageBox(_T("Failed to save the file."));
				break;
			}

			if(pU8Buffer)
			{
				delete[] pU8Buffer;
				pU8Buffer = nullptr;
			}

			pChild->SetWindowTextW(strCurTitle);
			m_bViewFirstChanged = m_bViewChanged = false;
			m_bSelectedFile = true;
			pChild->UpdateWindow();
			m_strTitle = strCurTitle;

			if(pMain->m_pFormView)
				pMain->m_pFormView->UpdateInfor(this);
		}
	}
	while(false);
}
void CMDIPainterView::OnViewClose()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();

	pChild->OnViewClose();
}
void CMDIPainterView::OnThickPx1()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();

	m_i32PxCost = 0;
	pMain->m_comboboxLineThick.SetCurSel(0);
}
void CMDIPainterView::OnUpdateThickPx1(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_i32PxCost == 0);
}
void CMDIPainterView::OnThickPx3()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();

	m_i32PxCost = 1;
	pMain->m_comboboxLineThick.SetCurSel(1);
}
void CMDIPainterView::OnUpdateThickPx3(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_i32PxCost == 1);
}
void CMDIPainterView::OnThickPx5()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();

	m_i32PxCost = 2;
	pMain->m_comboboxLineThick.SetCurSel(2);
}
void CMDIPainterView::OnUpdateThickPx5(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_i32PxCost == 2);
}
void CMDIPainterView::OnColorBlack()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();

	m_i32ColorCost = EColor_Black;
	m_colorrefCost = RGB(0, 0, 0);
	pMain->m_comboboxColor.SetCurSel(m_i32ColorCost);
}
void CMDIPainterView::OnUpdateColorBlack(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_i32ColorCost == EColor_Black);
}
void CMDIPainterView::OnColorRed()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();

	m_i32ColorCost = EColor_Red;
	m_colorrefCost = RGB(255, 0, 0);
	pMain->m_comboboxColor.SetCurSel(m_i32ColorCost);
}
void CMDIPainterView::OnUpdateColorRed(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_i32ColorCost == EColor_Red);
}
void CMDIPainterView::OnColorLime()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();

	m_i32ColorCost = EColor_Lime;
	m_colorrefCost = RGB(0, 255, 0);
	pMain->m_comboboxColor.SetCurSel(m_i32ColorCost);
}
void CMDIPainterView::OnUpdateColorLime(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_i32ColorCost == EColor_Lime);
}
void CMDIPainterView::OnColorBlue()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();

	m_i32ColorCost = EColor_Blue;
	m_colorrefCost = RGB(0, 0, 255);
	pMain->m_comboboxColor.SetCurSel(m_i32ColorCost);
}
void CMDIPainterView::OnUpdateColorBlue(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_i32ColorCost == EColor_Blue);
}
void CMDIPainterView::OnColorUserdef()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	CColorDialog colordialogTmp;

	if(colordialogTmp.DoModal() == IDOK)
	{
		m_colorrefCost = colordialogTmp.GetColor();

		if(m_colorrefCost == RGB(0, 0, 0))
			m_i32ColorCost = EColor_Black;
		else if(m_colorrefCost == RGB(255, 0, 0))
			m_i32ColorCost = EColor_Red;
		else if(m_colorrefCost == RGB(0, 255, 0))
			m_i32ColorCost = EColor_Lime;
		else if(m_colorrefCost == RGB(0, 0, 255))
			m_i32ColorCost = EColor_Blue;
		else
			m_i32ColorCost = EColor_UserDef;
	}

	pMain->m_comboboxColor.SetCurSel(m_i32ColorCost);
}
void CMDIPainterView::OnUpdateColorUserdef(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_i32ColorCost == EColor_UserDef);
}
void CMDIPainterView::OnBnClickedLineThick()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	int32_t i32Cur = pMain->m_comboboxLineThick.GetCurSel();

	m_i32PxCost = i32Cur;
}
void CMDIPainterView::OnBnClickedColor()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	int32_t i32Cur = pMain->m_comboboxColor.GetCurSel();

	if(i32Cur == 0)
		OnColorBlack();
	else if(i32Cur == 1)
		OnColorRed();
	else if(i32Cur == 2)
		OnColorLime();
	else if(i32Cur == 3)
		OnColorBlue();
	else if(i32Cur == 4)
		OnColorUserdef();
}
void CMDIPainterView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMenu menuPop, menuThick, menuColor, menuNew;

	m_bLButtonPushed = m_bMButtonPushed = false;
	menuThick.CreatePopupMenu();
	menuColor.CreatePopupMenu();
	menuPop.CreatePopupMenu();

	if(m_i32PxCost == 0)
		menuThick.AppendMenuW(MF_STRING | MF_CHECKED, ID_THICK_PX1, _T("&1 px"));
	else
		menuThick.AppendMenuW(MF_STRING | MF_UNCHECKED, ID_THICK_PX1, _T("&1 px"));

	if(m_i32PxCost == 1)
		menuThick.AppendMenuW(MF_STRING | MF_CHECKED, ID_THICK_PX3, _T("&3 px"));
	else
		menuThick.AppendMenuW(MF_STRING | MF_UNCHECKED, ID_THICK_PX3, _T("&3 px"));

	if(m_i32PxCost == 2)
		menuThick.AppendMenuW(MF_STRING | MF_CHECKED, ID_THICK_PX5, _T("&5 px"));
	else
		menuThick.AppendMenuW(MF_STRING | MF_UNCHECKED, ID_THICK_PX5, _T("&5 px"));

	if(m_i32ColorCost == EColor_Black)
		menuColor.AppendMenuW(MF_STRING | MF_CHECKED, ID_COLOR_BLACK, _T("BLAC&K"));
	else
		menuColor.AppendMenuW(MF_STRING | MF_UNCHECKED, ID_COLOR_BLACK, _T("BLAC&K"));

	if(m_i32ColorCost == EColor_Red)
		menuColor.AppendMenuW(MF_STRING | MF_CHECKED, ID_COLOR_RED, _T("LIGHT&RED"));
	else
		menuColor.AppendMenuW(MF_STRING | MF_UNCHECKED, ID_COLOR_RED, _T("LIGHT&RED"));

	if(m_i32ColorCost == EColor_Lime)
		menuColor.AppendMenuW(MF_STRING | MF_CHECKED, ID_COLOR_LIME, _T("&LIME"));
	else
		menuColor.AppendMenuW(MF_STRING | MF_UNCHECKED, ID_COLOR_LIME, _T("&LIME"));

	if(m_i32ColorCost == EColor_Blue)
		menuColor.AppendMenuW(MF_STRING | MF_CHECKED, ID_COLOR_BLUE, _T("&BLUE"));
	else
		menuColor.AppendMenuW(MF_STRING | MF_UNCHECKED, ID_COLOR_BLUE, _T("&BLUE"));

	if(m_i32ColorCost == EColor_UserDef)
		menuColor.AppendMenuW(MF_STRING | MF_CHECKED, ID_COLOR_USERDEF, _T("&User defined"));
	else
		menuColor.AppendMenuW(MF_STRING | MF_UNCHECKED, ID_COLOR_USERDEF, _T("&User defined"));

	menuNew.CreatePopupMenu();
	menuNew.AppendMenuW(MF_STRING, ID_FILE_NEW, _T("Paint View"));

	if(!pMain->m_bFormView)
		menuNew.AppendMenuW(MF_STRING, ID_FORMVIEW_NEW, _T("Information View"));
	else
		menuNew.AppendMenuW(MF_STRING | MF_GRAYED, ID_FORMVIEW_NEW, _T("Information View"));

	menuPop.AppendMenuW(MF_POPUP | MF_STRING, (UINT_PTR)menuNew.GetSafeHmenu(), _T("새로 만들기(&N)"));
	menuPop.AppendMenuW(MF_SEPARATOR);
	menuPop.AppendMenuW(MF_STRING, ID_FILE_OPEN, _T("열기(&O)"));

	if(m_bViewChanged)
		menuPop.AppendMenuW(MF_STRING, ID_FILE_SAVE, _T("저장(&S)"));
	else
		menuPop.AppendMenuW(MF_STRING | MF_GRAYED, ID_FILE_SAVE, _T("저장(&S)"));

	menuPop.AppendMenuW(MF_STRING, ID_VIEW_CLOSE, _T("닫기(&C)"));
	menuPop.AppendMenuW(MF_SEPARATOR);
	menuPop.AppendMenuW(MF_POPUP | MF_STRING, (UINT_PTR)menuThick.GetSafeHmenu(), _T("선 두께(&T)"));
	menuPop.AppendMenuW(MF_POPUP | MF_STRING, (UINT_PTR)menuColor.GetSafeHmenu(), _T("선 색(&R)"));

	menuPop.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}
BOOL CMDIPainterView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	ScreenToClient(&pt);
	
	SPoint ptTmp((double)pt.x, (double)pt.y, m_i32PxCost, m_colorrefCost), ptCursor;

	ptCursor = ScreenToReal(ptTmp);

	if(zDelta > 0)
	{
		if(m_f64ZoomRate * 1.5 > 100000.0)
			m_f64ZoomRate = 100000.0;
		else
			m_f64ZoomRate *= 1.5;
	}
	else
	{
		if(m_f64ZoomRate * 0.75 < 0.00001)
			m_f64ZoomRate = 0.00001;
		else
			m_f64ZoomRate *= 0.75;
	}

	ptTmp = RealToScreen(ptCursor);
	m_ptOffset.f64X += (double)pt.x - ptTmp.f64X;
	m_ptOffset.f64Y += (double)pt.y - ptTmp.f64Y;

	if(pMain->m_pFormView)
		pMain->m_pFormView->UpdateInfor(this);

	Invalidate(false);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
void CMDIPainterView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	if(m_bMButtonPushed)
	{
		m_ptOffset.f64X += (double)point.x - m_ptPanStart.f64X;
		m_ptOffset.f64Y += (double)point.y - m_ptPanStart.f64Y;
		m_ptPanStart.f64X = (double)point.x, m_ptPanStart.f64Y = (double)point.y;

		if(pMain->m_pFormView)
			pMain->m_pFormView->UpdateInfor(this);
	}

	if(m_bLButtonPushed)
	{
		SPoint ptTmp((double)point.x, (double)point.y, m_i32PxCost, m_colorrefCost);
		m_bViewChanged = m_bViewFirstChanged = true;
		m_vctLine[(int32_t)m_vctLine.size() - 1].push_back(ScreenToReal(ptTmp));
	}

	Invalidate(false);

	CView::OnMouseMove(nFlags, point);
}
void CMDIPainterView::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_bMButtonPushed = true;

	if(!GetCapture())
		SetCapture();

	m_ptPanStart.f64X = (double)point.x, m_ptPanStart.f64Y = (double)point.y;
	Invalidate(false);

	CView::OnMButtonDown(nFlags, point);
}
void CMDIPainterView::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_bMButtonPushed = false;

	if(!m_bLButtonPushed && !m_bMButtonPushed)
		ReleaseCapture();

	Invalidate(false);

	CView::OnMButtonUp(nFlags, point);
}
void CMDIPainterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLButtonPushed = m_bViewChanged = true;

	if(!GetCapture())
		SetCapture();

	SPoint ptTmp((double)point.x, (double)point.y, m_i32PxCost, m_colorrefCost);
	std::vector<SPoint> vctTmp;

	m_vctLine.push_back(vctTmp);
	m_vctLine[(int32_t)m_vctLine.size() - 1].push_back(ScreenToReal(ptTmp));
	SetModifiedTitle();
	Invalidate(false);

	CView::OnLButtonDown(nFlags, point);
}
void CMDIPainterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLButtonPushed = false;

	if(!m_bLButtonPushed && !m_bMButtonPushed)
		ReleaseCapture();

	Invalidate(false);

	CView::OnLButtonUp(nFlags, point);
}
void CMDIPainterView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	if(m_i32WindowSizeKey < 1)
	{
		int32_t i32X = (cx + 1) / 2, i32Y = (cy + 1) / 2;

		if(!m_bFirstWindowSizeChange)
		{
			m_ptWindowSizePrev.f64X = i32X;
			m_ptWindowSizePrev.f64Y = i32Y;
			m_bFirstWindowSizeChange = true;
		}
		else
		{
			m_ptOffset.f64X += (i32X - m_ptWindowSizePrev.f64X);
			m_ptOffset.f64Y += (i32Y - m_ptWindowSizePrev.f64Y);
			m_ptWindowSizePrev.f64X = i32X;
			m_ptWindowSizePrev.f64Y = i32Y;

			if(pMain->m_pFormView)
				pMain->m_pFormView->UpdateInfor(this);
		}
	}
	else
		m_i32WindowSizeKey--;

	Invalidate(false);
}
bool CMDIPainterView::SetModifiedTitle()
{
	bool bReturn = false;

	do
	{
		if(!m_bViewFirstChanged)
		{
			CString strCurTitle;
			CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
			CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();

			pChild->GetWindowTextW(strCurTitle);
			pChild->SetWindowTextW(('*' + strCurTitle));
			pMain->m_wndToolBarChild.SetButtonInfo(3, ID_FILE_SAVE, TBSTATE_ENABLED, 3);
			pMain->UpdateWindow();
			m_bViewFirstChanged = true;
			m_strTitle = '*' + strCurTitle;

			if(pMain->m_pFormView)
				pMain->m_pFormView->UpdateInfor(this);
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}
void CMDIPainterView::OnPx1()
{
	OnThickPx1();
}
void CMDIPainterView::OnPx3()
{
	OnThickPx3();
}
void CMDIPainterView::OnPx5()
{
	OnThickPx5();
}
void CMDIPainterView::OnBlack()
{
	OnColorBlack();
}
void CMDIPainterView::OnRed()
{
	OnColorRed();
}
void CMDIPainterView::OnLime()
{
	OnColorLime();
}
void CMDIPainterView::OnBlue()
{
	OnColorBlue();
}
bool CMDIPainterView::ChkInside(SPoint ptIn, double i32Right, double i32Bottom)
{
	return ptIn.f64X >= -50.0 && i32Right + 50.0 >= ptIn.f64X
		&& ptIn.f64Y >= -50.0 && i32Bottom + 50.0 >= ptIn.f64Y;
}
double CMDIPainterView::Ccw(double f64X1, double f64Y1, double f64X2, double f64Y2, double f64X3, double f64Y3)
{
	double f64Cross = ((f64X2 - f64X1) * (f64Y3 - f64Y1)) - ((f64X3 - f64X1) * (f64Y2 - f64Y1));
	double f64Return;

	if(f64Cross > 0.0)
		f64Return = 1;
	else if(f64Cross < 0.0)
		f64Return = -1;
	else
		f64Return = 0;

	return f64Return;
}
bool CMDIPainterView::GetCrossPoint(SPoint ptFirstStart, SPoint ptFirstEnd, SPoint ptSecondStart, SPoint ptSecondEnd, SPoint* pPtReturn)
{
	bool bReturn = true;
	double f64First, f64Second, f64FirstTmp, f64SecondTmp, f64Under;

	do
	{
		f64Under = ((ptSecondEnd.f64Y - ptSecondStart.f64Y) * (ptFirstEnd.f64X - ptFirstStart.f64X)) - ((ptSecondEnd.f64X - ptSecondStart.f64X) * (ptFirstEnd.f64Y - ptFirstStart.f64Y));

		if(f64Under == 0)
			break;

		f64FirstTmp = ((ptSecondEnd.f64X - ptSecondStart.f64X) * (ptFirstStart.f64Y - ptSecondStart.f64Y)) - ((ptSecondEnd.f64Y - ptSecondStart.f64Y) * (ptFirstStart.f64X - ptSecondStart.f64X));
		f64SecondTmp = ((ptFirstEnd.f64X - ptFirstStart.f64X) * (ptFirstStart.f64Y - ptSecondStart.f64Y)) - ((ptFirstEnd.f64Y - ptFirstStart.f64Y) * (ptFirstStart.f64X - ptSecondStart.f64X));
		f64First = f64FirstTmp / f64Under;
		f64Second = f64SecondTmp / f64Under;
		pPtReturn->f64X = ptFirstStart.f64X + (f64First * (ptFirstEnd.f64X - ptFirstStart.f64X));
		pPtReturn->f64Y = ptFirstStart.f64Y + (f64First * (ptFirstEnd.f64Y - ptFirstStart.f64Y));

		bReturn = true;
	}
	while(false);

	return bReturn;
}
uint32_t CMDIPainterView::GetCrc(uint8_t* pU8Buffer, uint32_t u32NByte)
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
BOOL CMDIPainterView::OnEraseBkgnd(CDC* pDC)
{
	return true;
}
void  CMDIPainterView::OnFormViewNew()
{
	CMDIPainterApp* pApp = (CMDIPainterApp*)AfxGetApp();

	pApp->OnFormViewNew();
}
