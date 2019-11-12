#pragma once

class CListItem : public CListContainerElementUI
{
private:
	bool HandleNotify(void* param);
	static DWORD WINAPI OnStart(LPVOID lpParam);

public:
	CListItem(POINT pt, int pl, int cs, int lor);
	void InitSubControls(HWND m_dhWnd);
	void ChangeConfig(HWND hWnd, POINT pt, int pl, int cs, int lor);
	bool IsRepeated(HWND hWnd, bool Change = false);

	static int GetAllItem();
	static void RemoveAllHwnd();
	void Start();
	void Stop(int Code);

private:
	POINT m_dPt;
	int m_dpl;
	int m_dcs;
	int m_dlor;
	CLabelUI* sb_title;
	CLabelUI* sb_pl;
	CLabelUI* sb_cs;
	CComboUI* sb_lor;
	CButtonUI* showWnd;
	CButtonUI* hideWnd;
	CButtonUI* xgTitle;
	CButtonUI* xgConfig;
	CButtonUI* sb_start;
	CButtonUI* sb_stop;
	CButtonUI* del_btn;
	HANDLE m_hThread;
};

