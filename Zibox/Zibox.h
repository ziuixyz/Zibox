#pragma once
#include "NotifyIcon.h"

class CZiboxWnd : public WindowImplBase
{
private:
	CNotifyIcon* Nin;
	ATOM Zi_SB_Add, Zi_SB_Start, Zi_SB_Stop;
public:
	void SetNin(CNotifyIcon* cNin) { Nin = cNin; }
public:
	virtual LPCTSTR    GetWindowClassName() const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	//void InitWindow();
	void RegisterHotKey();
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/) { return 0; };

	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};