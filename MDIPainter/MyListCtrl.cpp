#pragma warning(disable: 26454)
#include "pch.h"
#include "MDIPainter.h"
#include "MyListCtrl.h"


IMPLEMENT_DYNAMIC(MyListCtrl, CListCtrl)

MyListCtrl::MyListCtrl()
{
}
MyListCtrl::~MyListCtrl()
{
}

BEGIN_MESSAGE_MAP(MyListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &MyListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()


void MyListCtrl::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVCUSTOMDRAW* pSCustom = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR); 

    *pResult = 0;
    
    if(CDDS_PREPAINT == pSCustom->nmcd.dwDrawStage)
    {
        *pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if(CDDS_ITEMPREPAINT == pSCustom->nmcd.dwDrawStage)
    {
        COLORREF crText = RGB(0, 0, 0);
        COLORREF crTextBk = RGB(255, 255, 255);

        if(pSCustom->nmcd.dwItemSpec == 0)
        {
            crText = RGB(0, 0, 0);
            crTextBk = RGB(128, 128, 128);
        }

        pSCustom->clrText = crText;
        pSCustom->clrTextBk = crTextBk;

        *pResult = CDRF_DODEFAULT;
    }
}
