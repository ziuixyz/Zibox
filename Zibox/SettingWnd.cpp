#include "StdAfx.h"
#include "Zibox.h"
#include "SettingWnd.h"
#include "ConfigFile.h"

class CHotKeyUI : public CContainerUI
{
public:
	virtual LPCTSTR GetClass() const { return TEXT("HotKey"); }

	void Init() {
		CDialogBuilder builder;
		STRINGorID xml(IDR_HOTK);
		CControlUI* pUI = builder.Create(xml, TEXT("xml"));
		this->Add(pUI);
		this->SetFixedHeight(22);
		this->GetManager()->FindControl(TEXT("name"))->SetText(this->GetText());

		m_mod = static_cast<CComboUI*>(FindSubControl(TEXT("mod")));
		m_vk = static_cast<CComboUI*>(FindSubControl(TEXT("vk")));
	}

	int GetMod() {
		return m_mod->GetCurSel();
	}

	int GetVk() {
		return m_vk->GetCurSel();
	}

	void SetKey(int mod, int vk) {
		m_mod->SelectItem(mod);
		m_vk->SelectItem(vk);
	}

private:
	CComboUI* m_mod;
	CComboUI* m_vk;
};

CSettingWnd* m_pInstance;

CSettingWnd::CSettingWnd() { m_pInstance = this; }
CSettingWnd::~CSettingWnd() { m_pInstance = NULL; }
CSettingWnd* CSettingWnd::GetInstance() { return m_pInstance; }

LPCTSTR CSettingWnd::GetWindowClassName() const { return TEXT("Zi_Setting"); }
UILIB_RESOURCETYPE CSettingWnd::GetResourceType() const { return UILIB_RESOURCE; }
CDuiString CSettingWnd::GetSkinFile() { return TEXT("1000"); }
CDuiString CSettingWnd::GetSkinFolder() { return TEXT(""); }
void CSettingWnd::OnFinalMessage(HWND hWnd) { delete this; }

void CSettingWnd::Init(CZiboxWnd* Owner)
{
	m_Owner = Owner;
	Create(Owner->GetHWND(),TEXT("设置"),NULL,NULL);
	CenterWindow();

	m_WndShadow = new CWndShadow();
	RECT rcCorner = { 12,12,12,12 };
	RECT rcHoleOffset = { 0,0,0,0 };
	m_WndShadow->SetImage(8851, rcCorner, rcHoleOffset);
	m_WndShadow->Create(m_hWnd);

	hk1 = static_cast<CHotKeyUI*>(m_PaintManager.FindControl(TEXT("hk1")));
	hk2 = static_cast<CHotKeyUI*>(m_PaintManager.FindControl(TEXT("hk2")));
	hk3 = static_cast<CHotKeyUI*>(m_PaintManager.FindControl(TEXT("hk3")));

	CConfigFile Config;
	hk1->SetKey(
		Config.GetConfigInt(TEXT("HotKey"), TEXT("SB_AddMod"), 0),
		Config.GetConfigInt(TEXT("HotKey"), TEXT("SB_AddVk"), 8)
	);
	hk2->SetKey(
		Config.GetConfigInt(TEXT("HotKey"), TEXT("SB_StartMod"), 0),
		Config.GetConfigInt(TEXT("HotKey"), TEXT("SB_StartVk"), 9)
	);
	hk3->SetKey(
		Config.GetConfigInt(TEXT("HotKey"), TEXT("SB_StopMod"), 0),
		Config.GetConfigInt(TEXT("HotKey"), TEXT("SB_StopVk"), 10)
	);

	m_PaintManager.FindControl(TEXT("savebtn"))->SetEnabled(false);
}

CControlUI* CSettingWnd::CreateControl(LPCTSTR pstrClass)
{
	CControlUI* pUI = NULL;
	if (_tcsicmp(pstrClass, _T("HotKey")) == 0)
	{
		pUI = new CHotKeyUI;
	}
	return pUI;
}

void CSettingWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == TEXT("click")) {
		if (msg.pSender->GetName() == TEXT("savebtn")) {
			CConfigFile Config;
			CDuiString str;
			str.Format(TEXT("%d"), hk1->GetMod());
			Config.SetConfigString(TEXT("HotKey"), TEXT("SB_AddMod"), str.GetData());
			str.Format(TEXT("%d"), hk2->GetMod());
			Config.SetConfigString(TEXT("HotKey"), TEXT("SB_StartMod"), str.GetData());
			str.Format(TEXT("%d"), hk3->GetMod());
			Config.SetConfigString(TEXT("HotKey"), TEXT("SB_StopMod"), str.GetData());
			str.Format(TEXT("%d"), hk1->GetVk());
			Config.SetConfigString(TEXT("HotKey"), TEXT("SB_AddVk"), str.GetData());
			str.Format(TEXT("%d"), hk2->GetVk());
			Config.SetConfigString(TEXT("HotKey"), TEXT("SB_StartVk"), str.GetData());
			str.Format(TEXT("%d"), hk3->GetVk());
			Config.SetConfigString(TEXT("HotKey"), TEXT("SB_StopVk"), str.GetData());
			if (!CConfigFile::IsConfigFile()) {
				MessageBox(m_hWnd, TEXT("无法读取配置文件"), TEXT("错误"), MB_OK | MB_ICONERROR);
				return;
			}
			m_Owner->RegisterHotKey();
			m_PaintManager.FindControl(TEXT("savebtn"))->SetEnabled(false);
		}
		else if (msg.pSender->GetName() == TEXT("rebtn")) {
			CConfigFile::RemoveConfig();
			m_Owner->RegisterHotKey();
			hk1->SetKey(0, 8);
			hk2->SetKey(0, 9);
			hk3->SetKey(0, 10);
			m_PaintManager.FindControl(TEXT("savebtn"))->SetEnabled(false);
		}
		else if (msg.pSender->GetName() == TEXT("closebtn")) {
			Close();
		}
		return;
	}
	if (msg.sType == TEXT("itemselect")) {
		CControlUI* c = m_PaintManager.FindControl(TEXT("savebtn"));
		if (!c->IsEnabled()) {
			m_PaintManager.FindControl(TEXT("savebtn"))->SetEnabled(true);
		}
		return;
	}
}

LRESULT CSettingWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SETTEXT:
		//m_PaintManager.FindControl(TEXT("title"))->SetText((LPCTSTR)lParam);
		return __super::HandleMessage(uMsg, wParam, lParam);
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return 0;
}
