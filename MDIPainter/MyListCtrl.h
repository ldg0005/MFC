#pragma once


// MyListCtrl

class MyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(MyListCtrl)

public:
	MyListCtrl();
	virtual ~MyListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
};


