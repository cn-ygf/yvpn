#include "stdafx.h"
#include "hostlistitem.h"


CHostListItem::CHostListItem(CPaintManagerUI& paint_manager) :CListExItem(paint_manager, _T("host_list_item.xml"))
{
	this->m_Type = 0;
}


CHostListItem::~CHostListItem()
{
}



CString CHostListItem::GetTypeText()
{
	return this->m_TypeText;
}
CString CHostListItem::GetUsername()
{
	return this->m_Username;
}
CString CHostListItem::GetPassword()
{
	return this->m_Password;
}
CString CHostListItem::GetIpSecKey()
{
	return this->m_IpSecKey;
}
int CHostListItem::GetType()
{
	return this->m_Type;
}

void CHostListItem::SetTypeText(CString szText)
{
	this->m_TypeText = szText;
}
void CHostListItem::SetUsername(CString szText)
{
	this->m_Username = szText;
}
void CHostListItem::SetPassword(CString szText)
{
	this->m_Password = szText;
}
void CHostListItem::SetIpSecKey(CString szText)
{
	this->m_IpSecKey = szText;
}
void CHostListItem::SetType(int type)
{
	this->m_Type = type;
}


void CHostListItem::UpdateData(BOOL b)
{
	CListContainerElementUI *pListElement = GetListContainerElementUI();
	CButtonUI *pBtnLogo = (CButtonUI *)paint_manager_.FindSubControlByName(pListElement,_T("logo"));
	CTextUI *pTextHost = (CTextUI *)paint_manager_.FindSubControlByName(pListElement, _T("texthost"));
	CTextUI *pTextType = (CTextUI *)paint_manager_.FindSubControlByName(pListElement, _T("texttype"));
	CButtonUI* pDel = (CButtonUI *)paint_manager_.FindSubControlByName(pListElement, _T("delbtn"));
	CButtonUI* pEdit = (CButtonUI *)paint_manager_.FindSubControlByName(pListElement, _T("editbtn"));
	CButtonUI* pConn = (CButtonUI *)paint_manager_.FindSubControlByName(pListElement, _T("connbtn"));
	if (b)
	{
		pBtnLogo->SetBkImage(m_LogoText);
		pTextHost->SetText(m_HostText);
		pTextType->SetText(m_TypeText);
		CString name;
		name.Format(_T("delbtn_%d"), this->GetId());
		pDel->SetName(name);
		name.Format(_T("editbtn_%d"), this->GetId());
		pEdit->SetName(name);
		name.Format(_T("connbtn_%d"), this->GetId());
		pConn->SetName(name);
	}
}