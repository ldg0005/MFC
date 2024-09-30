#include "pch.h"
#include "MDIPainter.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "FormViewDoc.h"
#include "MDIPainterView.h"

// FormViewDoc
IMPLEMENT_DYNCREATE(FormViewDoc, CDocument)

FormViewDoc::FormViewDoc()
{
	//CMDIPainterView* pView = m_viewList.GetHead();
}
BOOL FormViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}
FormViewDoc::~FormViewDoc()
{
}
BEGIN_MESSAGE_MAP(FormViewDoc, CDocument)
END_MESSAGE_MAP()
// FormViewDoc 진단
#ifdef _DEBUG
void FormViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}
#ifndef _WIN32_WCE
void FormViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG
// FormViewDoc serialization
