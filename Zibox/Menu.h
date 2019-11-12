#pragma once

class CMenuWnd : public WindowImplBase
{
public:
	explicit CMenuWnd(UILIB_RESOURCETYPE ResType, LPCTSTR XMLName);

protected:
	virtual ~CMenuWnd();

public:
	virtual LPCTSTR    GetWindowClassName() const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual void    OnFinalMessage(HWND hWnd);

	void Init(CPaintManagerUI* pOwnerPM);
	void AdjustPostion();

	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	UILIB_RESOURCETYPE pszResType;
	LPCTSTR pszXMLName;
	CPaintManagerUI* m_pOwnerPM;
	HWND m_hWndParent;
};