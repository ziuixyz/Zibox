#include "StdAfx.h"
#include "NotifyIcon.h"

CNotifyIcon::CNotifyIcon(HINSTANCE hInst, HWND hWnd) : m_hInstance(hInst), m_hWnd(hWnd) {}

void CNotifyIcon::Create()
{
	m_msgNotify = {};
	m_msgNotify.cbSize = sizeof(NOTIFYICONDATA);
	m_msgNotify.hWnd = m_hWnd;
	m_msgNotify.uID = IDI_MAINICON;
	m_msgNotify.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	m_msgNotify.uCallbackMessage = WM_NOTIFYICON;
	m_msgNotify.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;;
	wcscpy_s(m_msgNotify.szTip, TEXT("Zi工具箱"));
	Shell_NotifyIcon(NIM_ADD, &m_msgNotify);
}

void CNotifyIcon::SendNotify(LPCTSTR Title, LPCTSTR Info)
{
	m_msgNotify.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	wcscpy_s(m_msgNotify.szInfoTitle, Title);
	wcscpy_s(m_msgNotify.szInfo, Info);
	Shell_NotifyIcon(NIM_MODIFY, &m_msgNotify);
}

void CNotifyIcon::Destroy()
{
	Shell_NotifyIcon(NIM_DELETE, &m_msgNotify);
}
