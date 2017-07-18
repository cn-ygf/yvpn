#include "stdafx.h"
#include "hostitemui.h"


CHostItemUI::CHostItemUI()
{
	this->m_Type = 0;
}


CHostItemUI::~CHostItemUI()
{
}


CDuiString CHostItemUI::GetHost()
{
	return this->m_Host;
}

CDuiString CHostItemUI::GetTypeText()
{
	return this->m_TypeText;
}
CDuiString CHostItemUI::GetRemark()
{
	return this->m_Remark;
}
CDuiString CHostItemUI::GetUsername()
{
	return this->m_Username;
}
CDuiString CHostItemUI::GetPassword()
{
	return this->m_Password;
}
CDuiString CHostItemUI::GetIpSecKey()
{
	return this->m_IpSecKey;
}
int CHostItemUI::GetType()
{
	return this->m_Type;
}

void CHostItemUI::SetHost(CDuiString szText)
{
	this->m_Host = szText;
}
void CHostItemUI::SetTypeText(CDuiString szText)
{
	this->m_TypeText = szText;
}
void CHostItemUI::SetRemark(CDuiString szText)
{
	this->m_Remark = szText;
}
void CHostItemUI::SetUsername(CDuiString szText)
{
	this->m_Username = szText;
}
void CHostItemUI::SetPassword(CDuiString szText)
{
	this->m_Password = szText;
}
void CHostItemUI::SetIpSecKey(CDuiString szText)
{
	this->m_IpSecKey = szText;
}
void CHostItemUI::SetType(int type)
{
	this->m_Type = type;
}


void CHostItemUI::UpdateData()
{
	CButtonUI *pBtnLogo = (CButtonUI *)this->m_pManager->FindControl(_T("btnlogo"));
	CTextUI *pTextHost = (CTextUI *)this->m_pManager->FindControl(_T("texthost"));
	CTextUI *pTextType = (CTextUI *)this->m_pManager->FindControl(_T("texttype"));
	pTextHost->SetText(_T("www.baidu.com"));
	pTextType->SetText(_T("PPTP"));

}