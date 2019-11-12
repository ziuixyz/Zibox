#pragma once

class CZhuaUI : public CButtonUI
{
public:
	virtual LPCTSTR GetClass() const;
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoEvent(TEventUI& event);

private:
	CDuiString def_image;
	CDuiString zhua_image;

};