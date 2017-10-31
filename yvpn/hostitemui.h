#pragma once

class CHostItemUI :
	public CControlUI
{
public:
	CHostItemUI();
	~CHostItemUI();
	CString GetHost();
	CString GetTypeText();
	CString GetRemark();
	CString GetUsername();
	CString GetPassword();
	CString GetIpSecKey();
	int GetType();

	void SetHost(CString szText);
	void SetTypeText(CString szText);
	void SetRemark(CString szText);
	void SetUsername(CString szText);
	void SetPassword(CString szText);
	void SetIpSecKey(CString szText);
	void SetType(int type);

	void UpdateData();
private:
	CString m_Host;
	CString m_TypeText;
	CString m_Remark;
	CString m_Username;
	CString m_Password;
	CString m_IpSecKey;
	int m_Type;
};

