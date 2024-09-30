#pragma once


// FromViewFrm 프레임

class FormViewFrm : public CFrameWnd
{
	DECLARE_DYNCREATE(FormViewFrm)
protected:
	FormViewFrm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~FormViewFrm();

protected:
	DECLARE_MESSAGE_MAP()
};


