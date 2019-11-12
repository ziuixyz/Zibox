#pragma once
#include "ListItem.h"

class CAddWindow : public WindowImplBase
{
public:
	explicit CAddWindow(UILIB_RESOURCETYPE ResType, LPCTSTR XMLName);

protected:
	virtual ~CAddWindow();

public:

	static bool IsInstance();
	static CAddWindow* GetInstance();

	virtual LPCTSTR    GetWindowClassName() const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual void    OnFinalMessage(HWND hWnd);

	void Init(CPaintManagerUI* Owner);
	void addWindow();
	void xgWindow(CListItem* xgItem, HWND hWnd, POINT pt, int pl, int cs, int lor);

	int GetScale();
	HDC StopFlash();
	HDC HighlightWindowBorder(HWND hWnd);

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	UILIB_RESOURCETYPE pszResType;
	LPCTSTR pszXMLName;
	CPaintManagerUI* m_pOwner;
	CWndShadow* m_WndShadow;
	CListItem* m_xgItem;
	HINSTANCE m_hInstance;
	HWND m_HighlightHwnd;
	HWND m_LastHwnd;
	int m_Scale;
	HWND sendhwnd;
};