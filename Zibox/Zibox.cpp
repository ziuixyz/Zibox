#include "StdAfx.h"
#include "Zibox.h"
#include "Menu.h"
#include "AddWindow.h"
#include "ListItem.h"

//资源ID转数字文本
CDuiString GetRes(int Res) {
	TCHAR szBuf[MAX_PATH] = { 0 };
	_stprintf_s(szBuf, MAX_PATH - 1, _T("%d"), Res);
	return szBuf;
}

LPCTSTR CZiboxWnd::GetWindowClassName() const { return TEXT("Zi_uixyz"); };
UILIB_RESOURCETYPE CZiboxWnd::GetResourceType() const { return UILIB_RESOURCE; }
CDuiString CZiboxWnd::GetSkinFile() { return GetRes(IDR_MAIN); };
CDuiString CZiboxWnd::GetSkinFolder() { return TEXT(""); }

void CZiboxWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == TEXT("click")) {
		if (msg.pSender->GetName() == TEXT("closebtn")) {
			ShowWindow(false);
			Nin->SendNotify(TEXT("Zi工具箱"), TEXT("我在这里哟！\n(●'◡'●)"));
			return;
		}
		else if (msg.pSender->GetName() == TEXT("minbtn")) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
		}
		else if (msg.pSender->GetName() == TEXT("menubtn")) {
			CMenuWnd* menu = new CMenuWnd(UILIB_RESOURCE, GetRes(IDR_MENU));
			menu->Init(&m_PaintManager);
			CControlUI* c = m_PaintManager.FindControl(TEXT("menubtn"));

			RECT w;
			::GetWindowRect(menu->GetHWND(), &w);
			POINT p;
			p.x = c->GetX() + c->GetWidth() - w.right;
			p.y = c->GetY() + c->GetHeight();

			ClientToScreen(m_hWnd, &p);
			::SetWindowPos(menu->GetHWND(), NULL, p.x, p.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
			return;
		}else if (msg.pSender->GetName() == TEXT("addbtn")) {
			CAddWindow* AdWnd;
			if (CAddWindow::IsInstance()) {
				AdWnd = CAddWindow::GetInstance();
				AdWnd->addWindow();
			}else{
				AdWnd = new CAddWindow(UILIB_RESOURCE, GetRes(IDR_ADDW));
				AdWnd->Init(&m_PaintManager);
				AdWnd->ShowWindow(true);
			}
			return;
		}
	}
	else if (msg.sType == TEXT("selectchanged"))
	{
		CDuiString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(TEXT("chunk")));
		//m_PaintManager.FindControl;
		if (name == TEXT("opt1"))
			pControl->SelectItem(0);
		else if (name == TEXT("opt2"))
			pControl->SelectItem(1);
		else if (name == TEXT("opt3"))
			pControl->SelectItem(2);
		else if (name == TEXT("opt4"))
			pControl->SelectItem(3);
		return;
	}
	else if (msg.sType == TEXT("menu")) {
		if (msg.pSender->GetName() == TEXT("sb_list")) {
			CMenuWnd* listmenu = new CMenuWnd(UILIB_RESOURCE, GetRes(IDR_LMENU));
			listmenu->Init(&m_PaintManager);
			listmenu->AdjustPostion();
			//listmenu->ShowWindow(true);
		}
	}
	else if (msg.sType == TEXT("menu_about")) {
		MessageBox(m_hWnd, TEXT("懒得写!\n问题反馈: 809511930"), TEXT("关于"), MB_OK | MB_ICONASTERISK);
		return;
	}
	else if (msg.sType == TEXT("im_start")) {
		int allitem = CListItem::GetAllItem();
		if (allitem) {
			CListUI* sb_List = static_cast<CListUI*>(m_PaintManager.FindControl(TEXT("sb_list")));
			while (allitem--) {
				CListItem* item = static_cast<CListItem*>(sb_List->GetItemAt(allitem));
				item->Stop(1);
				item->Start();
			}
		}
		else {
			MessageBox(m_hWnd, TEXT("没有添加项！"), TEXT("错误"), MB_OK);
		}
		return;
	}
	else if (msg.sType == TEXT("im_stop")) {
		int allitem = CListItem::GetAllItem();
		if (allitem) {
			CListUI* sb_List = static_cast<CListUI*>(m_PaintManager.FindControl(TEXT("sb_list")));
			while (allitem--) {
				CListItem* item = static_cast<CListItem*>(sb_List->GetItemAt(allitem));
				item->Stop(1);
			}
		}
		else {
			MessageBox(m_hWnd, TEXT("没有添加项！"), TEXT("错误"), MB_OK);
		}
		return;
	}
	else if (msg.sType == TEXT("im_remove")) {

		int allitem = CListItem::GetAllItem();
		if (allitem) {
			CListUI* sb_List = static_cast<CListUI*>(m_PaintManager.FindControl(TEXT("sb_list")));
			while (allitem--) {
				CListItem* item = static_cast<CListItem*>(sb_List->GetItemAt(allitem));
				item->Stop(1);
			}
			CListItem::RemoveAllHwnd();
			sb_List->RemoveAll();
		}
		else {
			MessageBox(m_hWnd, TEXT("没有添加项！"), TEXT("错误"), MB_OK);
		}
		return;
	}
}



LRESULT CZiboxWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//case WM_LBUTTONDOWN:
		//{
			//
			//CMenuWnd* pMenu = new CMenuWnd();
			//break;
		//}
	case WM_NCRBUTTONUP:
	{
		POINT pt;
		GetCursorPos(&pt);
		HMENU sMenu = GetSystemMenu(m_hWnd, FALSE);
		EnableMenuItem(sMenu, SC_MOVE, FALSE);
		EnableMenuItem(sMenu, SC_MINIMIZE, FALSE);
		EnableMenuItem(sMenu, SC_CLOSE, FALSE);

		EnableMenuItem(sMenu, SC_RESTORE, TRUE);
		EnableMenuItem(sMenu, SC_MAXIMIZE, 0);
		EnableMenuItem(sMenu, SC_SIZE, TRUE);
		//EnableMenuItem
		switch (TrackPopupMenu(sMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL))
		{
		case SC_CLOSE:
			PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
			break;
		case SC_MINIMIZE:
			PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			break;
		case SC_MAXIMIZE:
			PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			break;
		case SC_MOVE:
			PostMessage(WM_SYSCOMMAND, SC_MOVE, 0);
			break;
		default:
			break;
		}
		break;
	}
	case WM_NOTIFYICON://托盘图标信息
		switch (lParam)
		{
		case NIN_BALLOONUSERCLICK://单击通知
			Sleep(300);
		case WM_LBUTTONUP://左键托盘图标 弹起
			ShowWindow(true);
			break;
		case WM_RBUTTONUP://右键托盘图标 弹起
			CMenuWnd* menu = new CMenuWnd(UILIB_RESOURCE, GetRes(IDR_MENU));
			menu->Init(&m_PaintManager);
			menu->AdjustPostion();
			//pMenu->ShowWindow(true);
			break;
		}
		break;
	case WM_SETTEXT:
		m_PaintManager.FindControl(TEXT("title"))->SetText((LPCTSTR)lParam);
		return __super::HandleMessage(uMsg, wParam, lParam);
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return 0;
}
;