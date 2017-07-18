#pragma once

class CHostItemUI :
	public CControlUI
{
public:
	CHostItemUI();
	~CHostItemUI();
	CDuiString GetHost();
	CDuiString GetTypeText();
	CDuiString GetRemark();
	CDuiString GetUsername();
	CDuiString GetPassword();
	CDuiString GetIpSecKey();
	int GetType();

	void SetHost(CDuiString szText);
	void SetTypeText(CDuiString szText);
	void SetRemark(CDuiString szText);
	void SetUsername(CDuiString szText);
	void SetPassword(CDuiString szText);
	void SetIpSecKey(CDuiString szText);
	void SetType(int type);

	void UpdateData();
private:
	CDuiString m_Host;
	CDuiString m_TypeText;
	CDuiString m_Remark;
	CDuiString m_Username;
	CDuiString m_Password;
	CDuiString m_IpSecKey;
	int m_Type;
};

