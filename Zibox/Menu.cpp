#include "StdAfx.h"
#include "Menu.h"

CMenuWnd::CMenuWnd(UILIB_RESOURCETYPE ResType, LPCTSTR XMLName) : pszResType(ResType), pszXMLName(XMLName) {}
CMenuWnd::~CMenuWnd() {}
LPCTSTR CMenuWnd::GetWindowClassName() const { return TEXT("ZI_uimenu"); }
UILIB_RESOURCETYPE CMenuWnd::GetResourceType() const { return pszResType; }
CDuiString CMenuWnd::GetSkinFile() { return pszXMLName; }
CDuiString CMenuWnd::GetSkinFolder() { return TEXT(""); }
void CMenuWnd::OnFinalMessage(HWND hWnd) { delete this; }

void CMenuWnd::Init(CPaintManagerUI* pOwnerPM)
{
	if (pOwnerPM == NULL) return;
	m_pOwnerPM = pOwnerPM;
	m_hWndParent = pOwnerPM->GetPaintWindow();
	Create(m_hWndParent, NULL, UI_WNDSTYLE_DIALOG, WS_EX_TOPMOST);
	ShowWindow(true);
	SetForegroundWindow(m_hWnd);
}

void CMenuWnd::AdjustPostion()
{
	POINT pt;
	GetCursorPos(&pt);
	RECT rcWnd;
	::GetWindowRect(m_hWnd, &rcWnd);
	int mWidth = pt.x;	//鼠标X坐标
	int mHeight = pt.y;	//鼠标Y坐标
	int wWidth = rcWnd.right - rcWnd.left;	//窗口宽度
	int wHeight = rcWnd.bottom - rcWnd.top;	//窗口高度
	int sWidth = GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度
	int sHeight = GetSystemMetrics(SM_CYSCREEN); //屏幕高度

	if (mWidth + wWidth > sWidth) pt.x = mWidth - wWidth;
	if (mHeight + wHeight > sHeight) pt.y = mHeight - wHeight;

	::SetWindowPos(m_hWnd, NULL, pt.x, pt.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

void CMenuWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_ITEMSELECT) {
		Close();
	}
	if (msg.sType == TEXT("itemclick")) {
		CDuiString msn = msg.pSender->GetName();
		if (msn == TEXT("menu_close")) {
			//Shell_NotifyIcon(NIM_DELETE, nIcon);
			::PostMessage(m_hWndParent, WM_SYSCOMMAND, SC_CLOSE, 0);
			//exit(0);
		}
		else if (msn == TEXT("menu_show")) {
			::ShowWindow(m_hWndParent, SW_SHOWNORMAL);
			SetForegroundWindow(m_hWndParent);
		}
		else if (msn == TEXT("menu_about")) {
			m_pOwnerPM->SendNotify(NULL, TEXT("menu_about"), 0, 0, true);
		}
		else if (msn == TEXT("im_start") || msn == TEXT("im_stop") || msn == TEXT("im_remove")) { 
			CControlUI* sb_List = m_pOwnerPM->FindControl(TEXT("sb_list"));
			m_pOwnerPM->SendNotify(sb_List, msn, 0, 0, true);
		}
	}
}

LRESULT CMenuWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KILLFOCUS:
		Close();
		break;
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return 0;
}
