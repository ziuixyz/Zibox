#pragma once
class CInputWnd : public WindowImplBase
{
private:
	virtual ~CInputWnd();

public:
	static CDuiString InputWnd(HWND hWnd, CDuiString initText);

	virtual LPCTSTR    GetWindowClassName() const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual void    OnFinalMessage(HWND hWnd);

	void Init(CDuiString str);

	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CWndShadow* m_WndShadow;

};

