#include "StdAfx.h"
#include "NotifyIcon.h"
#include "Zibox.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//防止多开
	HANDLE hZiboxMutex = CreateMutex(NULL, TRUE, TEXT("ZiboxMutex"));
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		HWND hwndMain = FindWindow(TEXT("Zi_uixyz"), NULL);//获取主窗体句柄
		if (hwndMain) {
			ShowWindow(hwndMain, SW_SHOWNORMAL);
			SetForegroundWindow(hwndMain);
		}
		CloseHandle(hZiboxMutex);
		return 0;
	}

	CPaintManagerUI::SetInstance(hInstance);
	CWndShadow::Initialize(hInstance);

	CZiboxWnd Zibox;
	Zibox.Create(NULL, TEXT("Zibox"), UI_WNDSTYLE_CONTAINER, WS_EX_WINDOWEDGE);
	Zibox.SetIcon(IDI_MAINICON);
	Zibox.CenterWindow();
	CNotifyIcon* Nin = new CNotifyIcon(hInstance, Zibox.GetHWND());
	Nin->Create();
	Zibox.SetNin(Nin);
	Zibox.RegisterHotKey();
	Zibox.ShowModal();
	Nin->Destroy();
	return 0;
}