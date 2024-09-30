#pragma once

// FormViewDoc 문서

class FormViewDoc : public CDocument
{
	DECLARE_DYNCREATE(FormViewDoc)

public:
	FormViewDoc();
	virtual ~FormViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
