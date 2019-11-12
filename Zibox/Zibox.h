#pragma once
#include "NotifyIcon.h"

class CZiboxWnd : public WindowImplBase
{
private:
	CNotifyIcon* Nin;
public:
	void SetNin(CNotifyIcon* cNin) { Nin = cNin; }
public:
	virtual LPCTSTR    GetWindowClassName() const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	//void InitWindow();
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/) { return 0; };

	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};