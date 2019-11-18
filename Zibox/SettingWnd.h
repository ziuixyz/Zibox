#pragma once

class CHotKeyUI;

class CSettingWnd : public WindowImplBase
{
protected:
	virtual ~CSettingWnd();

public:
	CSettingWnd();
	static CSettingWnd* GetInstance();

	virtual LPCTSTR    GetWindowClassName() const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual void    OnFinalMessage(HWND hWnd);

	void Init(CZiboxWnd* Owner);

	CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	CZiboxWnd* m_Owner;
	CWndShadow* m_WndShadow;
	CHotKeyUI* hk1;
	CHotKeyUI* hk2;
	CHotKeyUI* hk3;
};

