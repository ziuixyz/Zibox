#include "StdAfx.h"
#include "InputWnd.h"

CDuiString InputText;

CInputWnd::~CInputWnd() {}

CDuiString CInputWnd::InputWnd(HWND hWnd, CDuiString initText)
{
	InputText.Empty();
	CInputWnd* Wnd = new CInputWnd;
	Wnd->Create(hWnd, TEXT("ÊäÈë"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	Wnd->Init(initText);
	Wnd->CenterWindow();
	Wnd->ShowModal();
	return InputText;
}

LPCTSTR CInputWnd::GetWindowClassName() const { return TEXT("Zi_Input"); }
UILIB_RESOURCETYPE CInputWnd::GetResourceType() const { return UILIB_RESOURCE; }
CDuiString CInputWnd::GetSkinFile() { return TEXT("10011"); }
CDuiString CInputWnd::GetSkinFolder() { return TEXT(""); }
void CInputWnd::OnFinalMessage(HWND hWnd) { delete this; }

void CInputWnd::Init(CDuiString str)
{
	m_PaintManager.FindControl(TEXT("InputText"))->SetText(str);

	m_WndShadow = new CWndShadow();
	RECT rcCorner = { 12,12,12,12 };
	RECT rcHoleOffset = { 0,0,0,0 };
	m_WndShadow->SetImage(8851, rcCorner, rcHoleOffset);
	m_WndShadow->Create(m_hWnd);
}

void CInputWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK) {
		CDuiString msn = msg.pSender->GetName();
		if (msn == TEXT("closebtn")) {
			Close();
		}
		else if (msn == TEXT("okbtn")) {
			InputText = m_PaintManager.FindControl(TEXT("InputText"))->GetText();
			Close();
		}
		return;
	}
}

LRESULT CInputWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SETTEXT:
		//m_PaintManager.FindControl(TEXT("title"))->SetText((LPCTSTR)lParam);
		return __super::HandleMessage(uMsg, wParam, lParam);
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return 0;
}
