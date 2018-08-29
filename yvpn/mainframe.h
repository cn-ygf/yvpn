#pragma once

typedef struct _INFO
{
	int id;
	CString host;
	CString user;
	CString pass;
	CString szL2tpKey;
	CString remark;
	DWORD type;
	DWORD crypt;
	HRASCONN h;
}INFO, *PINFO;

typedef struct _CONFIG
{
	char dns1[16];
	char dns2[16];
}CONFIG, *PCONFIG;

class CMainFrame:
	public WindowImplBase
{
public:
	CMainFrame();
	~CMainFrame();
	virtual LPCTSTR    GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void Notify(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);

private:
	std::map<int, PINFO> m_HostList;
	CONFIG m_Config;
	void InitData();
	void Connect(int id);
	void Save();
	void Edit(int id);
	void ClearControl();
	int Add(LPSTR szHostName, LPSTR szUser, LPSTR szPass, DWORD type, LPSTR szL2tpKey, DWORD crypt, LPSTR szRemark);
	int Set(int id,LPSTR szHostName, LPSTR szUser, LPSTR szPass, DWORD type, LPSTR szL2tpKey, DWORD crypt, LPSTR szRemark);
	int Del(int id);
	CString GetPosition(char* lpszIpAddr);
	int m_EditId;
	int ConnectionVPN(LPCWSTR lpName, LPCWSTR lpUser, LPCWSTR lpPass, LPCWSTR lpHost,LPHRASCONN h, DWORD crypt);
	int ConnectionVPNXP(char* lpName, char* lpUser, char* lpPass, char* lpHost, LPHRASCONN h, DWORD crypt);
	int ConnectionL2tpVPNXP(char *lpName, char * lpUser, char * lpPass, char * lpHost, char * lpL2tpKey, LPHRASCONN h, DWORD crypt);
	int ConnectionL2tpVPN(LPCWSTR lpName, LPCWSTR lpUser, LPCWSTR lpPass, LPCWSTR lpHost, LPCWSTR lpL2tpKey, LPHRASCONN h, DWORD crypt);
	static void WINAPI RasDialFunc(_In_  HRASCONN hrasconn,
		_In_  UINT unMsg,
		_In_  RASCONNSTATE rascs,
		_In_  DWORD dwError,
		_In_  DWORD dwExtendedError);
	void ShowText(LPCTSTR szText);
	void ShowTextSetting(LPCTSTR szText);
	bool OnButtonEvent(void* param);
	BOOL m_bXp;
	void SaveConfig(PCONFIG pConfig);
	void ReadConfig(PCONFIG pConfig);
	void SetTypeSelect(int index);
	void SetCryptSelect(int index);
};

