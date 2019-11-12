#include "StdAfx.h"
#include "AddWindow.h"
#include "Zhua.h"

CAddWindow* m_pInstance;

CAddWindow::CAddWindow(UILIB_RESOURCETYPE ResType, LPCTSTR XMLName) : 
	pszResType(ResType),
	pszXMLName(XMLName),
	m_HighlightHwnd(NULL),
	m_LastHwnd(NULL),
	m_Scale(1)
{
	m_pInstance = this;
}

CAddWindow::~CAddWindow() { m_pInstance = NULL; }

bool CAddWindow::IsInstance() {
	if (m_pInstance == NULL) return false;
	else true;
}

CAddWindow* CAddWindow::GetInstance() { return m_pInstance; }

LPCTSTR CAddWindow::GetWindowClassName() const { return TEXT("ZI_uiAW"); }
UILIB_RESOURCETYPE CAddWindow::GetResourceType() const { return pszResType; }
CDuiString CAddWindow::GetSkinFile() { return pszXMLName; }
CDuiString CAddWindow::GetSkinFolder() { return TEXT(""); }
void CAddWindow::OnFinalMessage(HWND hWnd) { delete this; }

void CAddWindow::Init(CPaintManagerUI* pOwner)
{
	m_pOwner = pOwner;
	m_Scale = GetScale();
	Create(m_pOwner->GetPaintWindow(), TEXT("添加"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	CenterWindow();

	m_WndShadow = new CWndShadow();
	RECT rcCorner = { 12,12,12,12 };
	RECT rcHoleOffset = { 0,0,0,0 };
	m_WndShadow->SetImage(8851, rcCorner, rcHoleOffset);
	m_WndShadow->Create(m_hWnd);
}

void CAddWindow::addWindow()
{
	SetWindowText(m_hWnd, TEXT("添加"));
	m_PaintManager.FindControl(TEXT("ad_okbtn"))->SetName(TEXT("ad_okbtn"));
}

void CAddWindow::xgWindow(CListItem* xgItem, HWND hWnd, POINT pt, int pl, int cs, int lor) {
	m_xgItem = xgItem;
	SetWindowText(m_hWnd, TEXT("修改"));
	m_PaintManager.FindControl(TEXT("ad_okbtn"))->SetName(TEXT("xg_okbtn"));

	CDuiString str;
	WCHAR wszTitle[MAX_PATH] = { 0 };
	GetWindowText(hWnd, wszTitle, MAX_PATH);
	str.Format(TEXT("%s"), wszTitle);
	m_PaintManager.FindControl(TEXT("ad_htitle"))->SetText(str);
	str.Format(TEXT("0x%08X"), hWnd);
	m_PaintManager.FindControl(TEXT("ad_handle"))->SetText(str);
	str.Format(TEXT("%d"), pt.x);
	m_PaintManager.FindControl(TEXT("ad_ptx"))->SetText(str);
	str.Format(TEXT("%d"), pt.y);
	m_PaintManager.FindControl(TEXT("ad_pty"))->SetText(str);
	str.Format(TEXT("%d"), pl);
	m_PaintManager.FindControl(TEXT("ad_pl"))->SetText(str);
	str.Format(TEXT("%d"), cs);
	m_PaintManager.FindControl(TEXT("ad_cs"))->SetText(str);
	CComboUI* ad_lor = static_cast<CComboUI*>(m_PaintManager.FindControl(TEXT("ad_lor")));
	ad_lor->SelectItem(lor);
	sendhwnd = hWnd;
}

int CAddWindow::GetScale() {
	HWND hWnd = GetDesktopWindow();
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);
	int cxLogical = miex.rcMonitor.right - miex.rcMonitor.left;
	int cyLogical = miex.rcMonitor.bottom - miex.rcMonitor.top;
	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
	int cxPhysical = dm.dmPelsWidth;
	int cyPhysical = dm.dmPelsHeight;
	int horzScale = cxPhysical * 100 / cxLogical;
	int vertScale = cyPhysical * 100 / cyLogical;
	return horzScale;
}

HDC CAddWindow::StopFlash()
{
	HDC hdc = NULL;
	if (m_HighlightHwnd)
	{
		hdc = HighlightWindowBorder(m_HighlightHwnd);
		m_HighlightHwnd = NULL;
	}
	return hdc;
}

HDC CAddWindow::HighlightWindowBorder(HWND hWnd)
{
	HDC nDc = GetWindowDC(hWnd);
	SetROP2(nDc, R2_NOTXORPEN);
	RECT rect;
	GetWindowRect(hWnd, &rect);
	OffsetRect(&rect, -rect.left, -rect.top);
	HPEN nHpen = CreatePen(0, 5, RGB(0, 0, 255));
	HGDIOBJ nHg = SelectObject(nDc, nHpen);
	Rectangle(nDc, rect.left, rect.top, rect.right * m_Scale / 100, rect.bottom * m_Scale / 100);
	SelectObject(nDc, nHg);
	DeleteObject(nHpen);
	ReleaseDC(hWnd, nDc);

	return nDc;
}

CControlUI* CAddWindow::CreateControl(LPCTSTR pstrClass)
{
	CControlUI* pUI = NULL;
	if (_tcsicmp(pstrClass, _T("Zhua")) == 0)
	{
		pUI = new CZhuaUI;
	}
	return pUI;
}

void CAddWindow::Notify(TNotifyUI& msg)
{
	if (msg.sType == TEXT("click")) {
		CDuiString msn = msg.pSender->GetName();
		if (msn == TEXT("closebtn")) {
			Close();
			return;
		}
		if (msn == TEXT("ad_okbtn") || msn == TEXT("xg_okbtn")) { 
			if (!sendhwnd) {
				MessageBox(m_hWnd, TEXT("你还没有捕获到窗口哦！"), TEXT("错误"), MB_OK | MB_ICONERROR);
				return;
			}
			if (!IsWindow(sendhwnd)) {
				MessageBox(m_hWnd, TEXT("找不到目标窗口！"), TEXT("错误"), MB_OK | MB_ICONERROR);
				return;
			}
			int ptx = _ttoi(m_PaintManager.FindControl(TEXT("ad_ptx"))->GetText());
			int pty = _ttoi(m_PaintManager.FindControl(TEXT("ad_pty"))->GetText());
			if (ptx < 0 || pty < 0) {
				MessageBox(m_hWnd, TEXT("坐标只能在目标窗口客户区内！"), TEXT("错误"), MB_OK | MB_ICONERROR);
				return;
			}
			int adpl = _ttoi(m_PaintManager.FindControl(TEXT("ad_pl"))->GetText());
			int adcs = _ttoi(m_PaintManager.FindControl(TEXT("ad_cs"))->GetText());
			POINT pt = { ptx , pty };
			CComboUI* ad_lor = static_cast<CComboUI*>(m_PaintManager.FindControl(TEXT("ad_lor")));
			int adlor = ad_lor->GetCurSel();

			if (msn == TEXT("ad_okbtn")) {
				CListUI* sb_List = static_cast<CListUI*>(m_pOwner->FindControl(TEXT("sb_list")));
				CListItem* sb_ListItem = new CListItem(pt, adpl, adcs, adlor);
				if (sb_ListItem->IsRepeated(sendhwnd)) {
					delete sb_ListItem;
					MessageBox(m_hWnd, TEXT("目标窗口已存在！"), TEXT("错误"), MB_OK | MB_ICONERROR);
					return;
				}
				sb_ListItem->InitSubControls(sendhwnd);
				sb_List->Add(sb_ListItem);
				Close();	//点击确定后 关闭 暂时不关调试用
				return;
			}
			else if (msn == TEXT("xg_okbtn")) {
				if (m_xgItem->IsRepeated(sendhwnd, true)) {
					MessageBox(m_hWnd, TEXT("目标窗口已存在！"), TEXT("错误"), MB_OK | MB_ICONERROR);
					return;
				}
				m_xgItem->ChangeConfig(sendhwnd, pt, adpl, adcs, adlor);
				Close();	//点击确定后 关闭 暂时不关调试用
				return;
			}
		}
		return;
	}
	else if (msg.sType == TEXT("textchanged")) {
		CDuiString str;
		CEditUI* ad_pl = static_cast<CEditUI*>(msg.pSender);
		int apl = abs(_ttoi(ad_pl->GetText()));
		str.Format(TEXT("%d"), apl);
		if (ad_pl->GetText() != str) {
			msg.pSender->SetText(str);
			ad_pl->SetSel(str.GetLength(), str.GetLength());
		}
		return;
	}
	else if (msg.sType == TEXT("Zhua_LBD")) {
		COptionUI* COpt = static_cast<COptionUI*>(m_PaintManager.FindControl(TEXT("ad_opth")));
		if (COpt->IsSelected()) ::ShowWindow(m_pOwner->GetPaintWindow(),SW_MINIMIZE);
		SetCapture(m_hWnd);
		SetCursor(LoadCursor(m_hInstance, MAKEINTRESOURCE(IDC_ZHUA)));
		return;
	}
	else if (msg.sType == TEXT("Zhua_LBU")) {
		COptionUI* COpt = static_cast<COptionUI*>(m_PaintManager.FindControl(TEXT("ad_opth")));
		if (COpt->IsSelected()) ::ShowWindow(m_pOwner->GetPaintWindow(), SW_SHOWNORMAL);
		ReleaseCapture();
		StopFlash();
		return;
	}
	else if (msg.sType == TEXT("Zhua_MM")) {
		HWND hWnd = NULL;
		if (GetCapture()) {
			POINT pt = msg.ptMouse;
			ClientToScreen(m_hWnd, &pt);
			//GetCursorPos(&pt);
			hWnd = WindowFromPoint(pt);
			ScreenToClient(hWnd, &pt);
			CDuiString str;
			WCHAR wszTitle[MAX_PATH] = { 0 };
			GetWindowText(hWnd, wszTitle, MAX_PATH);
			str.Format(TEXT("%s"), wszTitle);
			m_PaintManager.FindControl(TEXT("ad_htitle"))->SetText(str);
			str.Format(TEXT("0x%08X"), hWnd);
			m_PaintManager.FindControl(TEXT("ad_handle"))->SetText(str);
			str.Format(TEXT("%d"), pt.x);
			m_PaintManager.FindControl(TEXT("ad_ptx"))->SetText(str);
			str.Format(TEXT("%d"), pt.y);
			m_PaintManager.FindControl(TEXT("ad_pty"))->SetText(str);
			sendhwnd = hWnd;
		}
		if (hWnd != m_LastHwnd) {
			StopFlash();
			m_LastHwnd = hWnd;
			if (hWnd) {
				HighlightWindowBorder(hWnd);
				m_HighlightHwnd = hWnd;
			}
		}
		return;
	}
}

LRESULT CAddWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		m_hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		return __super::HandleMessage(uMsg, wParam, lParam);
	case WM_SETTEXT:
		m_PaintManager.FindControl(TEXT("title"))->SetText((LPCTSTR)lParam);
		return __super::HandleMessage(uMsg, wParam, lParam);
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return 0;
}
