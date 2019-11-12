#include "StdAfx.h"
#include "ListItem.h"
#include "AddWindow.h"
#include "InputWnd.h"

//存放数据
vector<HWND> allhWnd;

struct gjData
{
	CListItem* me;
	HWND hWnd;
};

CListItem::CListItem(POINT pt,int pl,int cs, int lor) : 
	m_dPt(pt),
	m_dpl(pl),
	m_dcs(cs),
	m_dlor(lor)
{}

DWORD WINAPI CListItem::OnStart(LPVOID lpParam) {
	struct gjData* data = (struct gjData*)lpParam;
	CListItem* me = data->me;
	HWND hWnd = data->hWnd;

	delete data;

	POINT pt = me->m_dPt;
	int  pl, cs, lor;
	pl = me->m_dpl;
	cs = me->m_dcs;
	lor = me->m_dlor;
	Sleep(1000);
	me->sb_stop->SetEnabled(true);

	int key1, key2;

	//左或右
	if (lor) key1 = WM_RBUTTONDOWN, key2 = WM_RBUTTONUP;
	else key1 = WM_LBUTTONDOWN, key2 = WM_LBUTTONUP; 	//0

	if (!pl) {
		while (IsWindow(hWnd)) {
			::PostMessage(hWnd, key1, 0, MAKELPARAM(pt.x, pt.y));
			Sleep(1000 * 30);
		}
		goto label;
	}

	//次数 频率
	if (cs) {
		while (cs--) {
			::PostMessage(hWnd, key1, 0, MAKELPARAM(pt.x, pt.y));
			::PostMessage(hWnd, key2, 0, MAKELPARAM(pt.x, pt.y));
			Sleep(pl);
			if (!IsWindow(hWnd)) break;
		}
	}
	else{
		while (IsWindow(hWnd)) {
			::PostMessage(hWnd, key1, 0, MAKELPARAM(pt.x, pt.y));
			::PostMessage(hWnd, key2, 0, MAKELPARAM(pt.x, pt.y));
			Sleep(pl);
		}
	}
	
	label:

	me->Stop(0);
	return 0;
}

int CListItem::GetAllItem() {
	//allhWnd.size();
	//CDuiString str;
	//str.Format(TEXT("%d"),allhWnd.size());
	//MessageBox(NULL, str, TEXT("1"), MB_OK);
	return allhWnd.size();
}

void CListItem::RemoveAllHwnd() {
	allhWnd.empty();
	allhWnd.resize(0);
}

void CListItem::Start() {
	CListUI* parent = static_cast<CListUI*>(this->GetParent());
	int nSel = parent->GetItemIndex(this);
	HWND hWnd = allhWnd.at(nSel);
	struct gjData* data = new gjData;
	data->me = this;
	data->hWnd = hWnd;
	sb_stop->SetEnabled(false);
	m_hThread = CreateThread(NULL, 0, &CListItem::OnStart, (LPVOID)data, 0, NULL);
	sb_start->SetVisible(false);
	sb_stop->SetVisible(true);
}

void CListItem::Stop(int Code) {
	if (!m_hThread) return;
	if (Code) {
		TerminateThread(m_hThread, 0);
		//CloseHandle(m_hThread);
	}
	sb_start->SetVisible(true);
	sb_stop->SetVisible(false);
}

void CListItem::InitSubControls(HWND hWnd)
{
	CDialogBuilder builder;
	STRINGorID xml(IDR_LITEM);
	CControlUI* pUI = builder.Create(xml, TEXT("xml"));
	this->SetFixedHeight(95);
	this->SetInset( { 15,15,15,15 } );
	this->Add(pUI);

	allhWnd.push_back(hWnd);

	sb_title = static_cast<CLabelUI*>(FindSubControl(TEXT("sb_title")));
	sb_pl = static_cast<CLabelUI*>(FindSubControl(TEXT("sb_pl")));
	sb_cs = static_cast<CLabelUI*>(FindSubControl(TEXT("sb_cs")));
	sb_lor = static_cast<CComboUI*>(FindSubControl(TEXT("sb_lor")));
	hideWnd = static_cast<CButtonUI*>(FindSubControl(TEXT("hidewnd")));
	showWnd = static_cast<CButtonUI*>(FindSubControl(TEXT("showwnd")));
	xgTitle = static_cast<CButtonUI*>(FindSubControl(TEXT("xgtitle")));
	xgConfig = static_cast<CButtonUI*>(FindSubControl(TEXT("xgconfig")));
	sb_start = static_cast<CButtonUI*>(FindSubControl(TEXT("sb_start")));
	sb_stop = static_cast<CButtonUI*>(FindSubControl(TEXT("sb_stop")));
	del_btn = static_cast<CButtonUI*>(FindSubControl(TEXT("del_btn")));

	CDuiString str;
	WCHAR wszTitle[MAX_PATH] = { 0 };
	GetWindowText(hWnd, wszTitle, MAX_PATH);
	str.Format(TEXT("标题: %s"), wszTitle);
	sb_title->SetText(str);
	str.Format(TEXT("频率: %dms"), m_dpl);
	sb_pl->SetText(str);
	if (m_dcs == 0) {
		sb_cs->SetText(TEXT("次数: 无限"));
	}else{
		str.Format(TEXT("次数: %d"), m_dcs);
		sb_cs->SetText(str);
	}
	sb_lor->SelectItem(m_dlor);

	//绑定事件
	sb_lor->OnNotify += MakeDelegate(this, &CListItem::HandleNotify);
	hideWnd->OnNotify += MakeDelegate(this, &CListItem::HandleNotify);
	showWnd->OnNotify += MakeDelegate(this, &CListItem::HandleNotify);
	xgTitle->OnNotify += MakeDelegate(this, &CListItem::HandleNotify);
	xgConfig->OnNotify += MakeDelegate(this, &CListItem::HandleNotify);
	sb_start->OnNotify += MakeDelegate(this, &CListItem::HandleNotify);
	sb_stop->OnNotify += MakeDelegate(this, &CListItem::HandleNotify);
	del_btn->OnNotify += MakeDelegate(this, &CListItem::HandleNotify);
}

void CListItem::ChangeConfig(HWND hWnd, POINT pt, int pl, int cs, int lor) {
	CListUI* parent = static_cast<CListUI*>(this->GetParent());
	*(allhWnd.begin() + parent->GetItemIndex(this)) = hWnd;

	m_dPt = pt;
	m_dpl = pl;
	m_dcs = cs;
	m_dlor = lor;

	CDuiString str;
	WCHAR wszTitle[MAX_PATH] = { 0 };
	GetWindowText(hWnd, wszTitle, MAX_PATH);
	str.Format(TEXT("标题: %s"), wszTitle);
	sb_title->SetText(str);
	str.Format(TEXT("频率: %dms"), m_dpl);
	sb_pl->SetText(str);
	if (m_dcs == 0) {
		sb_cs->SetText(TEXT("次数: 无限"));
	}
	else {
		str.Format(TEXT("次数: %d"), m_dcs);
		sb_cs->SetText(str);
	}
	sb_lor->SelectItem(m_dlor);
}

bool CListItem::IsRepeated(HWND hWnd, bool Change) {
	if (Change) {
		CListUI* parent = static_cast<CListUI*>(this->GetParent());
		if (allhWnd.at(parent->GetItemIndex(this)) == hWnd) return false;
	}
	vector<HWND>::iterator v;
	v = ::find(allhWnd.begin(), allhWnd.end(), hWnd);
	if (v != allhWnd.end()) {
		return true;
	}
	else { 
		return false; 
	}
}

bool CListItem::HandleNotify(void* param)
{
	TNotifyUI* msg = (TNotifyUI*)param;
	if (msg->sType == DUI_MSGTYPE_CLICK) {
		CListUI* parent = static_cast<CListUI*>(this->GetParent());
		int nSel = parent->GetItemIndex(this);
		HWND m_dhWnd = allhWnd.at(nSel);
		if (msg->pSender == del_btn) {
			Stop(1);
			parent->Remove(this);
			allhWnd.erase(allhWnd.begin() + nSel);
			return true;
		}
		else if (msg->pSender == xgConfig) {
			//CAddWindow* xgWindow = new CAddWindow(UILIB_RESOURCE, TEXT("10010"));
			CAddWindow* xgWindow;
			if (CAddWindow::IsInstance()) {
				xgWindow = CAddWindow::GetInstance();
				xgWindow->xgWindow(this, m_dhWnd, m_dPt, m_dpl, m_dcs, m_dlor);
			}
			else {
				xgWindow = new CAddWindow(UILIB_RESOURCE, TEXT("10010"));
				xgWindow->Init(this->GetManager());
				xgWindow->xgWindow(this, m_dhWnd, m_dPt, m_dpl, m_dcs, m_dlor);
				xgWindow->ShowWindow(true);
			}
			//CAddWindow* xgWindow = CAddWindow::GetInstance(UILIB_RESOURCE, TEXT("10010"));
			//xgWindow->Init(this->GetManager());
			//xgWindow->xgWindow(this, m_dhWnd, m_dPt, m_dpl, m_dcs, m_dlor);
			//xgWindow->ShowWindow(true);
			return true;
		}
		else if (!IsWindow(m_dhWnd)) {
			MessageBox(this->GetManager()->GetPaintWindow(), TEXT("目标窗口不存在！"), TEXT("错误"), MB_OK | MB_ICONERROR);
			return true;
		}
		else if (msg->pSender == sb_start) {
			Start();
			return true;
		}
		else if (msg->pSender == sb_stop) {
			Stop(1);
			return true;
		}
		else if (msg->pSender == hideWnd) {
			::ShowWindow(m_dhWnd, SW_HIDE); return true;
		}
		else if (msg->pSender == showWnd) {
			::ShowWindow(m_dhWnd, SW_SHOW); return true;
		}
		else if (msg->pSender == xgTitle) {
			WCHAR wszTitle[MAX_PATH] = { 0 };
			GetWindowText(m_dhWnd, wszTitle, MAX_PATH);
			CDuiString str = CInputWnd::InputWnd(this->GetManager()->GetPaintWindow(), wszTitle);
			if (str.IsEmpty()) return true;
			SetWindowText(m_dhWnd, str);
			str.Format(TEXT("标题: %s"), str);
			sb_title->SetText(str);
			return true;
		}
		return false;
	}
	else if (msg->sType == DUI_MSGTYPE_ITEMSELECT) {
		if (msg->pSender == sb_lor) {
			m_dlor = sb_lor->GetCurSel();
			return true;
		}
	}
	return false;
}