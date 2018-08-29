#pragma once

class CHostListItem :
	public CListExItem
{
public:
	CHostListItem(CPaintManagerUI& paint_manager);
	~CHostListItem();
	CString GetTypeText();
	CString GetRemark();
	CString GetUsername();
	CString GetPassword();
	CString GetIpSecKey();
	int GetType();
	void SetTypeText(CString szText);
	void SetRemark(CString szText);
	void SetUsername(CString szText);
	void SetPassword(CString szText);
	void SetIpSecKey(CString szText);
	void SetType(int type);
	int GetId(){ return m_Id; }
	int GetIndex(){ return m_Index; }
	CString GetHostText(){ return m_HostText; }
	CString GetPostionText(){ return m_PostionText; }
	CString GetRemarkText(){ return m_RemarkText; }
	CString GetLogoText(){ return m_LogoText; }
	void SetId(int id){ m_Id = id; }
	void SetIndex(int id){ m_Index = id; }
	void SetHostText(CString szText){ m_HostText = szText; }
	void SetPostionText(CString szText){ m_PostionText = szText; }
	void SetRemarkText(CString szText){ m_RemarkText = szText; }
	void SetLogoText(CString szText){ m_LogoText = szText; }
	virtual void UpdateData(BOOL b = TRUE);
private:
	CString m_TypeText;
	CString m_Username;
	CString m_Password;
	CString m_IpSecKey;
	int m_Id;
	int m_Index;
	CString m_HostText;
	CString m_PostionText;
	CString m_RemarkText;
	CString m_LogoText;
	int m_Type;
};

