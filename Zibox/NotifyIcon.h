#pragma once
class CNotifyIcon
{
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	NOTIFYICONDATA m_msgNotify;
public:
	CNotifyIcon(HINSTANCE hInst, HWND hWnd);
	void Create();
	void SendNotify(LPCTSTR Title, LPCTSTR Info);
	void Destroy();
};

