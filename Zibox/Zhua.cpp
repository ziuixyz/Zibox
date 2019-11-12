#include "StdAfx.h"
#include "Zhua.h"

LPCTSTR CZhuaUI::GetClass() const
{
	return TEXT("Zhua");
}

void CZhuaUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) {
	if (_tcscmp(pstrName, _T("defimage")) == 0) def_image = pstrValue, SetBkImage(pstrValue);
	else if (_tcscmp(pstrName, _T("zhuaimage")) == 0) zhua_image = pstrValue;
	else CButtonUI::SetAttribute(pstrName, pstrValue);
}

void CZhuaUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_BUTTONDOWN){
		m_pManager->SendNotify(this, TEXT("Zhua_LBD"), event.wParam, event.lParam, true);
		SetBkImage(zhua_image);
	}
	if (event.Type == UIEVENT_BUTTONUP) {
		m_pManager->SendNotify(this, TEXT("Zhua_LBU"), event.wParam, event.lParam, true);
		SetBkImage(def_image);
	}
	if (event.Type == UIEVENT_MOUSEMOVE) {
		m_pManager->SendNotify(this, TEXT("Zhua_MM"), event.wParam, event.lParam, true);
	}
	CButtonUI::DoEvent(event);
}