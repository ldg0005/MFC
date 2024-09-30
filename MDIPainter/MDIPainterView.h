#pragma once

#include "MyListCtrl.h"

enum EColor
{
	EColor_Black,
	EColor_Red,
	EColor_Lime,
	EColor_Blue,
	EColor_UserDef,
};
struct SPoint
{
	double f64X;
	double f64Y;
	int32_t i32Px;
	COLORREF colorrefColor;

	SPoint()
	{
		f64X = f64Y = 0.0;
		i32Px = 1;
		colorrefColor = RGB(0, 0, 0);
	}
	SPoint(double f64XIn, double f64YIn, int32_t i32In, COLORREF colorrefIn)
	{
		f64X = f64XIn;
		f64Y = f64YIn;
		i32Px = i32In;
		colorrefColor = colorrefIn;
	}

	SPoint& operator-(SPoint& ptValue)
	{
		f64X -= ptValue.f64X, f64Y -= ptValue.f64Y;
		return *this;
	}

	SPoint& operator+(SPoint& ptValue)
	{
		f64X += ptValue.f64X, f64Y += ptValue.f64Y;
		return *this;
	}
	operator CPoint()
	{
		for(int32_t i = 15; i >= 0; i--)
		{
			double f64TmpX, f64TmpY;

			f64TmpX = round(pow(10, i) * f64X) / pow(10, i);
			f64TmpY = round(pow(10, i) * f64Y) / pow(10, i);
			f64X = f64TmpX, f64Y = f64TmpY;
		}

		return CPoint((int32_t)(f64X), (int32_t)(f64Y));
	}
};

class CMDIPainterView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMDIPainterView() noexcept;
	DECLARE_DYNCREATE(CMDIPainterView)

// 특성입니다.
public:
	CMDIPainterDoc* GetDocument() const;
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CMDIPainterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	SPoint ScreenToReal(SPoint ptIn);
	SPoint RealToScreen(SPoint ptIn);
	bool ChkInside(SPoint ptIn, double i32Right, double i32Bottom);
	double Ccw(double f64X1, double f64Y1, double f64X2, double f64Y2, double f64X3, double f64Y3);
	bool GetCrossPoint(SPoint ptFirstStart, SPoint ptFirstEnd, SPoint ptSecondStart, SPoint ptSecondEnd, SPoint* pPtReturn);
	bool SetModifiedTitle();
	uint32_t GetCrc(uint8_t* pU8Buffer, uint32_t u32NByte);
	bool m_bViewChanged = false, m_bSelectedFile = false, m_bViewFirstChanged = false;
	std::vector<std::vector<SPoint> > m_vctLine;
	int32_t m_i32WindowSizeKey = 2, m_i32PxCost = 0, m_i32ColorCost = EColor_Black;
	uint32_t m_arrU32CrcTable[256];
	CString m_strPath, m_strTitle;
	SPoint m_ptPanStart, m_ptOffset, m_ptWindowSizePrev;
	double m_f64ZoomRate = 1.0;
	bool m_bLButtonPushed = false, m_bMButtonPushed = false, m_bFirstWindowSizeChange = false;
	COLORREF m_colorrefCost = RGB(0, 0, 0);
	CPen m_arrPenMyColor[3][5];

	MyListCtrl* m_pListView = nullptr;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnThickPx1();
	afx_msg void OnThickPx3();
	afx_msg void OnThickPx5();
	afx_msg void OnColorBlack();
	afx_msg void OnColorRed();
	afx_msg void OnColorLime();
	afx_msg void OnColorBlue();
	afx_msg void OnColorUserdef();
	afx_msg void OnFileNew();
	afx_msg void OnFileSave();
	afx_msg void OnUpdateThickPx1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateThickPx3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateThickPx5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorBlack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorRed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorLime(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorBlue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorUserdef(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnViewClose();
	afx_msg void OnPx1();
	afx_msg void OnPx3();
	afx_msg void OnPx5();
	afx_msg void OnBlack();
	afx_msg void OnRed();
	afx_msg void OnLime();
	afx_msg void OnBlue();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBnClickedLineThick();
	afx_msg void OnBnClickedColor();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFormViewNew();
	
};

#ifndef _DEBUG  // MDIPainterView.cpp의 디버그 버전
inline CMDIPainterDoc* CMDIPainterView::GetDocument() const
{
	return reinterpret_cast<CMDIPainterDoc*>(m_pDocument);
}
#endif

