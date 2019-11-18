#include "StdAfx.h"
#include "Zibox.h"
#include "Menu.h"
#include "AddWindow.h"
#include "ListItem.h"
#include "ConfigFile.h"
#include "SettingWnd.h"

//关于对话框的回调函数
INT_PTR CALLBACK DialogProc(
	HWND hWndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	switch (uMsg)
	{
	//处理静态控件的背景色
	case WM_CTLCOLORSTATIC:
		return (INT_PTR)CreateSolidBrush(RGB(255, 255, 255));
	//处理关于对话框的背景色
	case WM_ERASEBKGND:
		return true;
	//绘图
	case WM_PAINT:
	{
		PAINTSTRUCT pt;
		HDC hdc;
		HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAINICON), IMAGE_ICON, 256, 256, LR_DEFAULTCOLOR);
		hdc = BeginPaint(hWndDlg, &pt);
		//Rectangle(hdc, 20, 20, 160, 160);
		//DrawIcon(hdc, 20, 20, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAINICON)));
		DrawIconEx(hdc, 0, 0, hIcon, 160, 160, 0, NULL, DI_NORMAL);
		EndPaint(hWndDlg, &pt);
		return true;
	}
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_CLICK:
		case NM_RETURN:
			ShellExecute(NULL, L"open", ((PNMLINK)lParam)->item.szUrl, NULL, NULL, SW_SHOW);
			return true;
		}
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return true;
		}
	case WM_INITDIALOG:
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAINICON));
		::SendMessage(hWndDlg, WM_SETICON, (WPARAM)FALSE, (LPARAM)hIcon);
		ShowWindow(hWndDlg,SW_SHOWNORMAL);
		SetForegroundWindow(hWndDlg);
	}
	return false;
};

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

void CZiboxWnd::LoadConfig() {
	//初始化
	CConfigFile::Init();
	Zi_SB_Add = GlobalAddAtom(TEXT("Zi_SB_Add"));
	Zi_SB_Start = GlobalAddAtom(TEXT("Zi_SB_Start"));
	Zi_SB_Stop = GlobalAddAtom(TEXT("Zi_SB_Stop"));

	/*
	CConfigFile Config;
	if (!CConfigFile::IsConfigFile()) {
		Config.SetConfigString(TEXT("HotKey"), TEXT("SB_AddMod"), TEXT("0"));
		Config.SetConfigString(TEXT("HotKey"), TEXT("SB_AddVk"), TEXT("8"));
		Config.SetConfigString(TEXT("HotKey"), TEXT("SB_StartMod"), TEXT("0"));
		Config.SetConfigString(TEXT("HotKey"), TEXT("SB_StartVk"), TEXT("9"));
		Config.SetConfigString(TEXT("HotKey"), TEXT("SB_StopMod"), TEXT("0"));
		Config.SetConfigString(TEXT("HotKey"), TEXT("SB_StopVk"), TEXT("10"));
	}
	*/
	RegisterHotKey();
}

void CZiboxWnd::RegisterHotKey() {
	CConfigFile Config;
	UnregisterHotKey(m_hWnd, Zi_SB_Add);
	UnregisterHotKey(m_hWnd, Zi_SB_Start);
	UnregisterHotKey(m_hWnd, Zi_SB_Stop);
	::RegisterHotKey(m_hWnd, Zi_SB_Add,
		Config.GetHotKeyMod(TEXT("SB_AddMod"), NULL),
		Config.GetHotKeyVk(TEXT("SB_AddVk"), VK_F9)
	);
	::RegisterHotKey(m_hWnd, Zi_SB_Start,
		Config.GetHotKeyMod(TEXT("SB_StartMod"), NULL),
		Config.GetHotKeyVk(TEXT("SB_StartVk"), VK_F10)
	);
	::RegisterHotKey(m_hWnd, Zi_SB_Stop,
		Config.GetHotKeyMod(TEXT("SB_StopMod"), NULL),
		Config.GetHotKeyVk(TEXT("SB_StopVk"), VK_F11)
	);
}

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
			if (CAddWindow::GetInstance()) {
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
		DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUT), m_hWnd, (DLGPROC)DialogProc);
		//MessageBox(m_hWnd, TEXT("懒得写!\n问题反馈: 809511930"), TEXT("关于"), MB_OK | MB_ICONASTERISK);
		return;
	}
	else if (msg.sType == TEXT("menu_setting")) {
		if (!CSettingWnd::GetInstance()) {
			CSettingWnd* SetWnd = new CSettingWnd();
			SetWnd->Init(this);
			SetWnd->ShowWindow(true);
		}
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
			MessageBox(m_hWnd, TEXT("没有添加项！"), TEXT("错误"), MB_OK | MB_ICONERROR);
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
			MessageBox(m_hWnd, TEXT("没有添加项！"), TEXT("错误"), MB_OK | MB_ICONERROR);
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
			MessageBox(m_hWnd, TEXT("没有添加项！"), TEXT("错误"), MB_OK | MB_ICONERROR);
		}
		return;
	}
}

LRESULT CZiboxWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_HOTKEY:
		if (wParam == Zi_SB_Start) {
			int allitem = CListItem::GetAllItem();
			if (allitem) {
				PlaySound((LPCTSTR)IDR_START, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
				Nin->SendNotify(TEXT("Zi工具箱"),TEXT("鼠标连点器已启动"));
				//PlaySound(TEXT("E:\\Users\\msg_dw\\Desktop\\Zibox\\Zibox\\Zibox\\res\\SOUND\\SB_StartSound.wav"), NULL, SND_ASYNC);
				CListUI* sb_List = static_cast<CListUI*>(m_PaintManager.FindControl(TEXT("sb_list")));
				while (allitem--) {
					CListItem* item = static_cast<CListItem*>(sb_List->GetItemAt(allitem));
					item->Stop(1);
					item->Start();
				}
			}
			else {
				Nin->SendNotify(TEXT("Zi工具箱"), TEXT("没有添加项！"));
			}
		}
		else if (wParam == Zi_SB_Stop) {
			int allitem = CListItem::GetAllItem();
			if (allitem) {
				PlaySound((LPCTSTR)IDR_STOP, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
				Nin->SendNotify(TEXT("Zi工具箱"), TEXT("鼠标连点器已停止"));
				CListUI* sb_List = static_cast<CListUI*>(m_PaintManager.FindControl(TEXT("sb_list")));
				while (allitem--) {
					CListItem* item = static_cast<CListItem*>(sb_List->GetItemAt(allitem));
					item->Stop(1);
				}
			}
			else {
				Nin->SendNotify(TEXT("Zi工具箱"), TEXT("没有添加项！"));
			}
		}
		else if (wParam == Zi_SB_Add) {
			CAddWindow* AdWnd;
			if (CAddWindow::GetInstance()) {
				AdWnd = CAddWindow::GetInstance();
				AdWnd->addWindow();
				::ShowWindow(AdWnd->GetHWND(), SW_SHOWNORMAL);
			}
			else {
				AdWnd = new CAddWindow(UILIB_RESOURCE, GetRes(IDR_ADDW));
				AdWnd->Init(&m_PaintManager);
				AdWnd->ShowWindow(true);
			}
			SetForegroundWindow(AdWnd->GetHWND());
		}
		break;
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
	case WM_DESTROY:
		UnregisterHotKey(m_hWnd, Zi_SB_Add);
		UnregisterHotKey(m_hWnd, Zi_SB_Start);
		UnregisterHotKey(m_hWnd, Zi_SB_Stop);
		GlobalDeleteAtom(Zi_SB_Add);
		GlobalDeleteAtom(Zi_SB_Start);
		GlobalDeleteAtom(Zi_SB_Stop);
		return __super::HandleMessage(uMsg, wParam, lParam);
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return 0;
}
;