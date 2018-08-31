#include "stdafx.h"
#include "mainframe.h"
#include "hostlistitem.h"
#include "CppSQLite3.h"
#include "unit.h"
#include "IpFinder.h"


CMainFrame *m_pThis = NULL;

CMainFrame::CMainFrame()
{
	m_pThis = this;
	m_EditId = 0;
}


CMainFrame::~CMainFrame()
{
}


DUI_BEGIN_MESSAGE_MAP(CMainFrame, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()




CDuiString CMainFrame::GetSkinFile()
{
	return _T("main_frame.xml");
}

LPCTSTR	CMainFrame::GetWindowClassName() const
{
	return _T("DUIMainFrame");
}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	CString SenderName = msg.pSender->GetName();
	if (msg.sType == _T("windowinit"))
	{
		this->SetIcon(IDI_YVPN);

		ReadConfig(&m_Config);
		CEditUI *pDns1 = (CEditUI *)m_PaintManager.FindControl(_T("dns1edit"));
		wchar_t *lpszDn1 = LocalToUnicode(CP_ACP, m_Config.dns1);
		pDns1->SetText(lpszDn1);
		CEditUI *pDns2 = (CEditUI *)m_PaintManager.FindControl(_T("dns2edit"));
		wchar_t *lpszDn2 = LocalToUnicode(CP_ACP, m_Config.dns2);
		pDns2->SetText(lpszDn2);
		delete[]lpszDn1;
		delete[]lpszDn2;
		CComboUI *pMode = (CComboUI *)m_PaintManager.FindControl(_T("networkmode"));
		pMode->SelectItem(m_Config.dwMode);

		m_bXp = FALSE;
		OSVERSIONINFOEX os = { 0 };
		os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		GetVersionEx((OSVERSIONINFO*)&os);
		if (os.dwMajorVersion == 5)
		{
			m_bXp = TRUE;
		}


		CButtonUI *pCloseBtn = (CButtonUI*)m_PaintManager.FindControl(_T("closebtn"));
		CButtonUI *pMinBtn = (CButtonUI*)m_PaintManager.FindControl(_T("minbtn"));
		CButtonUI *pHostBtn = (CButtonUI*)m_PaintManager.FindControl(_T("hostbtn"));
		CButtonUI *pAddBtn = (CButtonUI*)m_PaintManager.FindControl(_T("addbtn"));
		CButtonUI *pSetBtn = (CButtonUI*)m_PaintManager.FindControl(_T("setbtn"));
		CButtonUI *pMenuBtn = (CButtonUI*)m_PaintManager.FindControl(_T("menubtn"));
		pCloseBtn->OnEvent += DuiLib::MakeDelegate(this, &CMainFrame::OnButtonEvent);
		pMinBtn->OnEvent += DuiLib::MakeDelegate(this, &CMainFrame::OnButtonEvent);
		pHostBtn->OnEvent += DuiLib::MakeDelegate(this, &CMainFrame::OnButtonEvent);
		pAddBtn->OnEvent += DuiLib::MakeDelegate(this, &CMainFrame::OnButtonEvent);
		pSetBtn->OnEvent += DuiLib::MakeDelegate(this, &CMainFrame::OnButtonEvent);
		pMenuBtn->OnEvent += DuiLib::MakeDelegate(this, &CMainFrame::OnButtonEvent);
		InitData();
	}
	else if (msg.sType == _T("itemclick"))
	{
		/*CHostListUI* pHostList = (CHostListUI *)(m_PaintManager.FindControl(_T("hosts")));
		int nSel = pHostList->GetCurSel();
		if (nSel > -1)
		{
			CString name;
			name.Format(_T("option_layout_%d"),nSel);
			CVerticalLayoutUI *pOption = (CVerticalLayoutUI *)pHostList->paint_manager_.FindControl(name);
			pOption->SetVisible(true);
		}*/
	}
	__super::Notify(msg);
}

bool CMainFrame::OnButtonEvent(void* param)
{
	TEventUI* pEvent = (TEventUI*)param;
	if (pEvent->Type == UIEVENT_SETCURSOR) {
		return false;
	}
	return true;
}

void CMainFrame::OnClick(TNotifyUI& msg)
{
	CString pSenderName = msg.pSender->GetName();
	if (pSenderName == _T("closebtn"))
	{
		if (MessageBox(GetHWND(),TEXT("是否真的退出程序？将挂断所有VPN！"), TEXT("提示"), MB_YESNO | MB_ICONINFORMATION) == IDNO)
		{
			return;
		}
		std::map <int, PINFO>::iterator iter;
		for (iter = m_pThis->m_HostList.begin(); iter != m_pThis->m_HostList.end(); iter++)
		{
			PINFO pInfo = iter->second;
			if (pInfo->h != NULL)
			{
				RasHangUp(pInfo->h);
			}
			delete pInfo;
		}
		PostQuitMessage(0);
	}
	else if (pSenderName == _T("minbtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	}
	else if (pSenderName == _T("hostbtn"))
	{
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("tab_bar"));
		pTabLayout->SelectItem(0);
	}
	else if (pSenderName == _T("addbtn"))
	{
		m_EditId = 0;
		ClearControl();
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("tab_bar"));
		pTabLayout->SelectItem(1);
		
	}
	else if (pSenderName == _T("setbtn"))
	{
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("tab_bar"));
		pTabLayout->SelectItem(2);
	}
	else if (pSenderName == _T("menubtn"))
	{
	}
	else if (pSenderName == _T("delbtn"))
	{
		CListExUI* pHostList = (CListExUI *)(m_PaintManager.FindControl(_T("hosts")));
		int nSel = pHostList->GetCurSel();
		int afd = 0;
	}
	else if (pSenderName == _T("savebtn"))
	{
		Save();
	}
	else if (pSenderName.Find(_T("editbtn")) > -1)
	{
		CString numberstr = ToNumberChar(pSenderName);
		int id = _wtoi(numberstr.GetBuffer());
		m_EditId = id;
		Edit(id);
	}
	else if (pSenderName.Find(_T("delbtn")) > -1)
	{
		if (MessageBox(GetHWND(),TEXT("是否真的删除？"), TEXT("提示"), MB_YESNO) == IDNO)
		{
			return;
		}
		CString numberstr = ToNumberChar(pSenderName);
		int id = _wtoi(numberstr.GetBuffer());
		Del(id);
		InitData();
	}
	else if (pSenderName.Find(_T("connbtn")) > -1)
	{
		CString numberstr = ToNumberChar(pSenderName);
		int id = _wtoi(numberstr.GetBuffer());
		Connect(id);
	}
	else if (pSenderName == _T("netsetbtn"))
	{
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("set_tab"));
		pTabLayout->SelectItem(0);
		CButtonUI *pBtn = (CButtonUI *)m_PaintManager.FindControl(_T("netsetbtn"));
		pBtn->SetTextColor(0xff1790ed);
		pBtn = (CButtonUI *)m_PaintManager.FindControl(_T("aboutbtn"));
		pBtn->SetTextColor(0xff000000);

		CVerticalLayoutUI *pLayout = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("net_layout"));
		pLayout->SetVisible(true);
		pLayout = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("pro_layout"));
		pLayout->SetVisible(false);
		pLayout = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("about_layout"));
		pLayout->SetVisible(false);
	}
	else if (pSenderName == _T("aboutbtn"))
	{
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("set_tab"));
		pTabLayout->SelectItem(2);
		CButtonUI * pBtn = (CButtonUI *)m_PaintManager.FindControl(_T("netsetbtn"));
		pBtn->SetTextColor(0xff000000);
		pBtn = (CButtonUI *)m_PaintManager.FindControl(_T("aboutbtn"));
		pBtn->SetTextColor(0xff1790ed);

		CVerticalLayoutUI *pLayout = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("net_layout"));
		pLayout->SetVisible(false);
		pLayout = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("pro_layout"));
		pLayout->SetVisible(false);
		pLayout = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("about_layout"));
		pLayout->SetVisible(true);
	}
	else if (pSenderName == _T("save2btn"))
	{
		CEditUI *pDns1 = (CEditUI *)m_PaintManager.FindControl(_T("dns1edit"));
		char *lpszDn1 = UnicodeToLocal(CP_ACP, pDns1->GetText());
		sprintf_s(m_Config.dns1,lpszDn1);
		CEditUI *pDns2 = (CEditUI *)m_PaintManager.FindControl(_T("dns2edit"));
		char *lpszDn2 = UnicodeToLocal(CP_ACP, pDns2->GetText());
		sprintf_s(m_Config.dns2, lpszDn2);
		delete[]lpszDn1;
		delete[]lpszDn2;
		CComboUI *pMode = (CComboUI *)m_PaintManager.FindControl(_T("networkmode"));
		m_Config.dwMode = pMode->GetCurSel();
		SaveConfig(&m_Config);
	}
}


void CMainFrame::InitData()
{
	CListExUI* pHostList = (CListExUI *)(m_PaintManager.FindControl(_T("hosts")));
	if (pHostList->GetCount() > 0)
	{
		pHostList->RemoveAll();
		std::map <int,PINFO>::iterator iter;
		for (iter = m_HostList.begin(); iter != m_HostList.end(); iter++)
		{
			delete iter->second;
		}
		m_HostList.clear();
	}
	CppSQLite3DB db;
	db.open(("yvpn.db"));
	CppSQLite3Query query = db.execQuery(("select * from vpn"));
	int i = 0;
	while (!query.eof())
	{
		int id = query.getIntField(("id"));
		const char* lpszHost = query.getStringField(("host"));
		const char* lpszUsername = query.getStringField(("username"));
		const char* lpszPassword = query.getStringField(("password"));
		int type = query.getIntField(("type"));
		int crypt = query.getIntField(("crypt"));
		const char* lpszSecKey = query.getStringField(("l2tpkey"));
		const char* lpszRemark = query.getStringField(("remark"));

		PINFO pInfo = new INFO;
		pInfo->h = NULL;
		pInfo->id = id;
		DWORD dwReadByte = 0;
		wchar_t *lpwtszUser = Utf8ToUnicode(lpszUsername, &dwReadByte);
		wchar_t *lpwtszPass = Utf8ToUnicode(lpszPassword, &dwReadByte);
		wchar_t *lpwtszL2tpkey = Utf8ToUnicode(lpszSecKey, &dwReadByte);

		pInfo->user.Append(lpwtszUser);
		pInfo->pass.Append(lpwtszPass);
		pInfo->szL2tpKey.Append(lpwtszL2tpkey);

		pInfo->type = type;
		pInfo->crypt = crypt;
		m_HostList[id] = pInfo;

		CString postion = GetPosition((char *)lpszHost);

		
		wchar_t *lpwtszHost = Utf8ToUnicode(lpszHost, &dwReadByte);
		pInfo->host.Append(lpwtszHost);
		CHostListItem item(m_PaintManager);
		item.SetId(id);
		item.SetIndex(i);
		item.SetHostText(lpwtszHost);
		delete[]lpwtszHost;
		if (type == 0)
			item.SetTypeText(_T("WAN Miniport (PPTP)"));
		else
			item.SetTypeText(_T("WAN Miniport (L2TP)"));
		item.SetPostionText(postion);
		wchar_t *lpwtszRemark = Utf8ToUnicode(lpszRemark, &dwReadByte);
		pInfo->remark.Append(lpwtszRemark);
		item.SetRemarkText(lpwtszRemark);
		delete[]lpwtszRemark;

		if (postion.Find(_T("省")) > -1 || postion.Find(_T("市")) > -1)
		{
			item.SetLogoText(_T("china.png"));
		}
		else if (postion.Compare(_T("香港")) == 0)
		{
			item.SetLogoText(_T("hongkong.png"));
		}
		else if (postion.Compare(_T("英国")) == 0)
		{
			item.SetLogoText(_T("eng.png"));
		}
		else if (postion.Compare(_T("美国")) == 0)
		{
			item.SetLogoText(_T("usa.png"));
		}
		else if (postion.Compare(_T("日本")) == 0)
		{
			item.SetLogoText(_T("jp.png"));
		}
		else if (postion.Compare(_T("韩国")) == 0)
		{
			item.SetLogoText(_T("hr.png"));
		}
		else if (postion.Compare(_T("加拿大")) == 0)
		{
			item.SetLogoText(_T("canada.png"));
		}
		else if (postion.Compare(_T("新加坡")) == 0)
		{
			item.SetLogoText(_T("sng.png"));
		}
		else if (postion.Compare(_T("印度")) == 0)
		{
			item.SetLogoText(_T("ri.png"));
		}
		else
		{
			item.SetLogoText(_T("china.png"));
		}
		item.UpdateData(TRUE);
		pHostList->AddNode(&item, NULL);

		query.nextRow();
		i++;
	}
	query.finalize();
	db.close();
}

CString CMainFrame::GetPosition(char* lpIPAddr)
{
	CIpFinder m_ipfind;
	if (!m_ipfind.Open("qqwry.dat"))
	{
		return CString(_T("qqwry.dat不存在"));
	}
	std::string country, location;
	std::string aa, bb;
	m_ipfind.GetAddressByIp(lpIPAddr, country, location);
	m_ipfind.Close();
	CString result;
	wchar_t *lpszCountry = LocalToUnicode(936, country.c_str());
	wchar_t *lpszLocation = LocalToUnicode(936, location.c_str());
	result.Format(_T("%s"), lpszCountry);
	delete[]lpszCountry;
	delete[]lpszLocation;
	return result;
}

void CMainFrame::Save()
{
	CEditUI *pHost = (CEditUI *)m_PaintManager.FindControl(_T("hostedit"));
	CComboUI *pType = (CComboUI *)m_PaintManager.FindControl(_T("typecombo"));
	CEditUI *pUsername = (CEditUI *)m_PaintManager.FindControl(_T("usernameedit"));
	CEditUI *pPassword = (CEditUI *)m_PaintManager.FindControl(_T("passwordedit"));
	CEditUI *pL2tpKey = (CEditUI *)m_PaintManager.FindControl(_T("l2tpkeyedit"));
	CComboUI *pCrypt = (CComboUI *)m_PaintManager.FindControl(_T("cryptcombo"));
	CEditUI *pRemark = (CEditUI *)m_PaintManager.FindControl(_T("remarkedit"));

	if (pHost->GetText().GetLength() < 1)
	{
		MessageBox(GetHWND(),_T("输入不正确！"),_T("提示"),MB_OK);
		return;
	}
	if (pUsername->GetText().GetLength() < 1)
	{
		MessageBox(GetHWND(), _T("输入不正确！"), _T("提示"), MB_OK);
		return;
	}
	if (pPassword->GetText().GetLength() < 1)
	{
		MessageBox(GetHWND(), _T("输入不正确！"), _T("提示"), MB_OK);
		return;
	}

	int type = 0;
	int crypt = 0;
	type = pType->GetCurSel();
	crypt = pCrypt->GetCurSel();

	DWORD dwReadByte = 0;
	char *lpszHost = UnicodeToUtf8(pHost->GetText().GetData(),&dwReadByte);
	char *lpszUser = UnicodeToUtf8(pUsername->GetText().GetData(), &dwReadByte);
	char *lpszPass = UnicodeToUtf8(pPassword->GetText().GetData(), &dwReadByte);
	char *lpszL2tp = UnicodeToUtf8(pL2tpKey->GetText().GetData(), &dwReadByte);
	char *lpszRemark = UnicodeToUtf8(pRemark->GetText().GetData(), &dwReadByte);

	int ret = 0;
	if (m_EditId > 0)
		ret = Set(m_EditId, lpszHost, lpszUser, lpszPass, type, lpszL2tp, crypt, lpszRemark);
	else
		ret = Add(lpszHost,lpszUser,lpszPass,type,lpszL2tp,crypt,lpszRemark);

	delete[]lpszHost;
	delete[]lpszUser;
	delete[]lpszPass;
	delete[]lpszL2tp;
	delete[]lpszRemark;

	if (ret > 0)
	{
		ClearControl();
		InitData();
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("tab_bar"));
		pTabLayout->SelectItem(0);
	}
}

void CMainFrame::Edit(int id)
{
	ClearControl();
	CEditUI *pHost = (CEditUI *)m_PaintManager.FindControl(_T("hostedit"));
	CComboUI *pType = (CComboUI *)m_PaintManager.FindControl(_T("typecombo"));
	CEditUI *pUsername = (CEditUI *)m_PaintManager.FindControl(_T("usernameedit"));
	CEditUI *pPassword = (CEditUI *)m_PaintManager.FindControl(_T("passwordedit"));
	CEditUI *pL2tpKey = (CEditUI *)m_PaintManager.FindControl(_T("l2tpkeyedit"));
	CComboUI *pCrypt = (CComboUI *)m_PaintManager.FindControl(_T("cryptcombo"));
	CEditUI *pRemark = (CEditUI *)m_PaintManager.FindControl(_T("remarkedit"));

	PINFO pInfo = m_HostList[id];

	pHost->SetText(pInfo->host);
	pUsername->SetText(pInfo->user);
	pPassword->SetText(pInfo->pass);
	pL2tpKey->SetText(pInfo->szL2tpKey);
	pRemark->SetText(pInfo->remark);

	pType->SelectItem(pInfo->type);
	pCrypt->SelectItem(pInfo->crypt);

	CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("tab_bar"));
	pTabLayout->SelectItem(1);
}

void CMainFrame::SetTypeSelect(int index)
{
	if (index == 0)
	{
		CString name;
		name.Format(_T("typecombo_%d"), 0);
		CListLabelElementUI *pListLabel = (CListLabelElementUI *)m_PaintManager.FindControl(name);
		if (pListLabel)
		{
			pListLabel->Select(true);
		}
		name.Format(_T("typecombo_%d"), 1);
		pListLabel = (CListLabelElementUI *)m_PaintManager.FindControl(name);
		if (pListLabel)
		{
			pListLabel->Select(false);
		}
	}
	else
	{
		CString name;
		name.Format(_T("typecombo_%d"), 0);
		CListLabelElementUI *pListLabel = (CListLabelElementUI *)m_PaintManager.FindControl(name);
		if (pListLabel)
		{
			pListLabel->Select(false);
		}
		name.Format(_T("typecombo_%d"), 1);
		pListLabel = (CListLabelElementUI *)m_PaintManager.FindControl(name);
		if (pListLabel)
		{
			pListLabel->Select(true);
		}
	}
	
}

void CMainFrame::SetCryptSelect(int index)
{
	CString name;
	name.Format(_T("cryptcombo_%d"),index);
	CListLabelElementUI *pListLabel = (CListLabelElementUI *)m_PaintManager.FindControl(name);
	if (pListLabel)
	{
		pListLabel->Select();
	}
}

int CMainFrame::Add(LPSTR szHostName,LPSTR szUser, LPSTR szPass, DWORD type,LPSTR szL2tpKey,DWORD crypt, LPSTR szRemark)
{
	char *sql = "insert into vpn (host,username,password,type,l2tpkey,crypt,remark) values ('%s','%s','%s','%d','%s','%d','%s')";
	DWORD len = 1024;
	char *lpSql = new char[len];
	ZeroMemory(lpSql, len);
	sprintf(lpSql, sql, szHostName, szUser, szPass,type,szL2tpKey,crypt, szRemark);
	int result = 0;
	CppSQLite3DB db;
	db.open("yvpn.db");
	result = db.execDML(lpSql);
	delete[]lpSql;
	db.close();
	return result;
}

int CMainFrame::Set(int id, LPSTR szHostName, LPSTR szUser, LPSTR szPass, DWORD type, LPSTR szL2tpKey, DWORD crypt, LPSTR szRemark)
{
	char *sql = "update vpn set host='%s',username='%s',password='%s',type='%d',l2tpkey='%s',crypt='%d',remark='%s' where id='%d'";
	DWORD len = 1024;
	char *lpSql = new char[len];
	ZeroMemory(lpSql, len);
	sprintf(lpSql, sql, szHostName, szUser, szPass, type, szL2tpKey, crypt, szRemark,id);
	int result = 0;
	CppSQLite3DB db;
	db.open("yvpn.db");
	result = db.execDML(lpSql);
	delete[]lpSql;
	db.close();
	return result;
}

int CMainFrame::Del(int id)
{
	char szSql[1024] = { 0 };
	sprintf(szSql, "delete from vpn where id=%d", id);
	int result = 0;
	CppSQLite3DB db;
	db.open("yvpn.db");
	result = db.execDML(szSql);
	db.close();
	return result;
}

void CMainFrame::ClearControl()
{
	CEditUI *pHost = (CEditUI *)m_PaintManager.FindControl(_T("hostedit"));
	CEditUI *pUsername = (CEditUI *)m_PaintManager.FindControl(_T("usernameedit"));
	CEditUI *pPassword = (CEditUI *)m_PaintManager.FindControl(_T("passwordedit"));
	CEditUI *pL2tpKey = (CEditUI *)m_PaintManager.FindControl(_T("l2tpkeyedit"));
	CEditUI *pRemark = (CEditUI *)m_PaintManager.FindControl(_T("remarkedit"));
	pHost->SetText(_T(""));
	pUsername->SetText(_T(""));
	pPassword->SetText(_T(""));
	pL2tpKey->SetText(_T(""));
	pRemark->SetText(_T(""));
}

int CMainFrame::ConnectionVPNXP(char* lpName, char* lpUser, char* lpPass, char* lpHost, LPHRASCONN h, DWORD crypt, DWORD mode)
{
	RASENTRYA re;
	RASCREDENTIALSA rc;
	ZeroMemory(&re, sizeof(RASENTRYA));
	ZeroMemory(&rc, sizeof(RASCREDENTIALSA));
	DWORD dwReSize = 0;
	RasGetEntryPropertiesA(NULL, NULL, NULL, &dwReSize, NULL, NULL);
	//re.dwSize = sizeof(RASENTRYA);
	re.dwSize = dwReSize;
	re.dwCountryCode = 86;
	re.dwCountryID = 86;
	re.dwDialExtraPercent = 10;
	re.dwDialExtraSampleSeconds = 120;
	re.dwDialMode = RASEDM_DialAll;
	re.dwEncryptionType = crypt;//加密方式;
	re.dwfNetProtocols = RASNP_Ip;
	//re.dwfOptions = RASEO_ModemLights | RASEO_RemoteDefaultGateway | RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	if (mode == 0)//网络模式
	{
		//局部模式
		re.dwfOptions = RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	}
	else
	{
		//全局模式
		re.dwfOptions = RASEO_RemoteDefaultGateway | RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	}
	re.dwfOptions2 = RASEO2_ReconnectIfDropped | RASEO2_DontNegotiateMultilink;
	re.dwFramingProtocol = RASFP_Ppp;
	re.dwHangUpExtraPercent = 10;
	re.dwHangUpExtraSampleSeconds = 120;
	re.dwIdleDisconnectSeconds = RASIDS_UseGlobalValue;
	re.dwRedialCount = 9999;
	re.dwRedialPause = 60;
	re.dwType = RASET_Vpn;
	re.dwVpnStrategy = VS_PptpOnly;
	unsigned long  dns, dns2;

	dns = inet_addr(m_Config.dns1);
	dns2 = inet_addr(m_Config.dns2);
	memcpy(&re.ipaddrDns, &dns, sizeof(dns));
	memcpy(&re.ipaddrDnsAlt, &dns2, sizeof(dns2));

	sprintf_s(re.szDeviceType, ("vpn"));
	sprintf_s(re.szDeviceName, ("WAN Miniport (PPTP)"));
	sprintf_s(re.szLocalPhoneNumber, (lpHost));
	DWORD dwResult = RasSetEntryPropertiesA(("conf.yvpn"), (lpName), &re, re.dwSize, NULL, 0);
	if (dwResult == 0)
	{
		rc.dwSize = sizeof(RASCREDENTIALSA);
		rc.dwMask = RASCM_UserName | RASCM_Password;
		sprintf_s(rc.szUserName, (lpUser));
		sprintf_s(rc.szPassword, (lpPass));
		dwResult = RasSetCredentialsA(("conf.yvpn"), (lpName), &rc, 0);

		if (dwResult == 0)
		{
			RASDIALPARAMSA ra;
			ZeroMemory(&ra, sizeof(RASDIALPARAMSA));
			sprintf_s(ra.szEntryName, (lpName));
			sprintf_s(ra.szUserName, (lpUser));
			sprintf_s(ra.szPassword, (lpPass));
			ra.dwSize = 1052;
			*h = NULL;
			dwResult = RasDialA(NULL, "conf.yvpn", &ra, 1, RasDialFunc, h);
		}

	}
	return dwResult;
}

int CMainFrame::ConnectionVPN(LPCWSTR lpName, LPCWSTR lpUser, LPCWSTR lpPass, LPCWSTR lpHost, LPHRASCONN h, DWORD crypt, DWORD mode)
{
	RASENTRY re;
	RASCREDENTIALS rc;
	ZeroMemory(&re, sizeof(RASENTRY));
	ZeroMemory(&rc, sizeof(RASCREDENTIALS));
	DWORD dwReSize = 0;
	RasGetEntryProperties(NULL, NULL, NULL, &dwReSize, NULL, NULL);
	//re.dwSize = sizeof(RASENTRYA);
	re.dwSize = dwReSize;
	re.dwCountryCode = 86;
	re.dwCountryID = 86;
	re.dwDialExtraPercent = 10;
	re.dwDialExtraSampleSeconds = 120;
	re.dwDialMode = RASEDM_DialAll;
	re.dwEncryptionType = crypt;//加密方式;
	re.dwfNetProtocols = RASNP_Ip;
	//re.dwfOptions = RASEO_ModemLights | RASEO_RemoteDefaultGateway | RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	if (mode == 0)//网络模式
	{
		//局部模式
		re.dwfOptions = RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	}
	else
	{
		//全局模式
		re.dwfOptions = RASEO_RemoteDefaultGateway | RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	}
	re.dwfOptions2 = RASEO2_ReconnectIfDropped | RASEO2_DontNegotiateMultilink;
	re.dwFramingProtocol = RASFP_Ppp;
	re.dwHangUpExtraPercent = 10;
	re.dwHangUpExtraSampleSeconds = 120;
	re.dwIdleDisconnectSeconds = RASIDS_UseGlobalValue;
	re.dwRedialCount = 9999;
	re.dwRedialPause = 60;
	re.dwType = RASET_Vpn;
	re.dwVpnStrategy = VS_PptpOnly;
	unsigned long  dns, dns2;

	dns = inet_addr(m_Config.dns1);
	dns2 = inet_addr(m_Config.dns2);
	memcpy(&re.ipaddrDns, &dns, sizeof(dns));
	memcpy(&re.ipaddrDnsAlt, &dns2, sizeof(dns2));

	wsprintf(re.szDeviceType, (L"vpn"));
	wsprintf(re.szDeviceName, (L"WAN Miniport (PPTP)"));
	//USES_CONVERSION;
	wsprintf(re.szLocalPhoneNumber, (lpHost));
	DWORD dwResult = RasSetEntryProperties((L"conf.yvpn"), (lpName), &re, re.dwSize, NULL, 0);
	if (dwResult == 0)
	{
		rc.dwSize = sizeof(RASCREDENTIALSA);
		rc.dwMask = RASCM_UserName | RASCM_Password;
		wsprintf(rc.szUserName, (lpUser));
		wsprintf(rc.szPassword, (lpPass));
		dwResult = RasSetCredentials((L"conf.yvpn"), (lpName), &rc, 0);

		if (dwResult == 0)
		{
			RASDIALPARAMS ra;
			ZeroMemory(&ra, sizeof(RASDIALPARAMS));
			wsprintf(ra.szEntryName, lpName);
			wsprintf(ra.szUserName, lpUser);
			wsprintf(ra.szPassword, lpPass);
			ra.dwSize = sizeof(RASDIALPARAMS);
			*h = NULL;
			dwResult = RasDial(NULL, L"conf.yvpn", &ra, 1, RasDialFunc, h);
		}

	}
	return dwResult;
}

int CMainFrame::ConnectionL2tpVPNXP(char *lpName, char * lpUser, char * lpPass, char * lpHost, char * lpL2tpKey, LPHRASCONN h, DWORD crypt, DWORD mode)
{
	RASENTRYA re;
	RASCREDENTIALSA rc;
	ZeroMemory(&re, sizeof(RASENTRYA));
	ZeroMemory(&rc, sizeof(RASCREDENTIALSA));
	DWORD dwReSize = 0;
	RasGetEntryPropertiesA(NULL, NULL, NULL, &dwReSize, NULL, NULL);
	//re.dwSize = sizeof(RASENTRYA);
	re.dwSize = dwReSize;
	re.dwCountryCode = 86;
	re.dwCountryID = 86;
	re.dwDialExtraPercent = 10;
	re.dwDialExtraSampleSeconds = 120;
	re.dwDialMode = RASEDM_DialAll;
	re.dwEncryptionType = crypt;//加密方式;
	re.dwfNetProtocols = RASNP_Ip;
	//re.dwfOptions = RASEO_ModemLights | RASEO_RemoteDefaultGateway | RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	if (mode == 0)//网络模式
	{
		//局部模式
		re.dwfOptions = RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	}
	else
	{
		//全局模式
		re.dwfOptions = RASEO_RemoteDefaultGateway | RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	}
	re.dwfOptions2 = RASEO2_ReconnectIfDropped | RASEO2_DontNegotiateMultilink | RASEO2_UsePreSharedKey;
	re.dwFramingProtocol = RASFP_Ppp;
	re.dwHangUpExtraPercent = 10;
	re.dwHangUpExtraSampleSeconds = 120;
	re.dwIdleDisconnectSeconds = RASIDS_UseGlobalValue;
	re.dwRedialCount = 9999;
	re.dwRedialPause = 60;
	re.dwType = RASET_Vpn;
	re.dwVpnStrategy = VS_L2tpOnly;
	unsigned long  dns, dns2;

	dns = inet_addr(m_Config.dns1);
	dns2 = inet_addr(m_Config.dns2);
	memcpy(&re.ipaddrDns, &dns, sizeof(dns));
	memcpy(&re.ipaddrDnsAlt, &dns2, sizeof(dns2));

	sprintf_s(re.szDeviceType, ("vpn"));
	sprintf_s(re.szDeviceName, ("WAN Miniport (L2TP)"));
	sprintf_s(re.szLocalPhoneNumber, (lpHost));
	DWORD dwResult = RasSetEntryPropertiesA(("conf.yvpn"), (lpName), &re, re.dwSize, NULL, 0);
	if (dwResult == 0)
	{
		rc.dwSize = sizeof(RASCREDENTIALSA);
		rc.dwMask = RASCM_UserName | RASCM_Password;
		sprintf_s(rc.szUserName, (lpUser));
		sprintf_s(rc.szPassword, (lpPass));
		dwResult = RasSetCredentialsA(("conf.yvpn"), (lpName), &rc, 0);

		memset(&rc, 0, sizeof(RASCREDENTIALSA));
		rc.dwSize = sizeof(RASCREDENTIALSA);
		rc.dwMask = RASCM_PreSharedKey;
		sprintf_s(rc.szPassword, lpL2tpKey);
		dwResult = RasSetCredentialsA(("conf.yvpn"), (lpName), &rc, 0);

		if (dwResult == 0)
		{
			RASDIALPARAMSA ra;
			ZeroMemory(&ra, sizeof(RASDIALPARAMSA));
			sprintf_s(ra.szEntryName, (lpName));
			sprintf_s(ra.szUserName, (lpUser));
			sprintf_s(ra.szPassword, (lpPass));
			ra.dwSize = 1052;
			*h = NULL;
			dwResult = RasDialA(NULL, "conf.yvpn", &ra, 1, RasDialFunc, h);
		}

	}
	return dwResult;
}

int CMainFrame::ConnectionL2tpVPN(LPCWSTR lpName, LPCWSTR lpUser, LPCWSTR lpPass, LPCWSTR lpHost, LPCWSTR lpL2tpKey, LPHRASCONN h, DWORD crypt,DWORD mode)
{
	RASENTRY re;
	RASCREDENTIALS rc;
	ZeroMemory(&re, sizeof(RASENTRY));
	ZeroMemory(&rc, sizeof(RASCREDENTIALS));
	DWORD dwReSize = 0;
	RasGetEntryProperties(NULL, NULL, NULL, &dwReSize, NULL, NULL);
	//re.dwSize = sizeof(RASENTRYA);
	re.dwSize = dwReSize;
	re.dwCountryCode = 86;
	re.dwCountryID = 86;
	re.dwDialExtraPercent = 10;
	re.dwDialExtraSampleSeconds = 120;
	re.dwDialMode = RASEDM_DialAll;
	re.dwEncryptionType = crypt;//加密方式;
	re.dwfNetProtocols = RASNP_Ip;
	//re.dwfOptions = RASEO_ModemLights | RASEO_RemoteDefaultGateway | RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	if (mode == 0)//网络模式
	{
		//局部模式
		re.dwfOptions = RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	}
	else
	{
		//全局模式
		re.dwfOptions = RASEO_RemoteDefaultGateway | RASEO_SpecificNameServers | RASEO_ShowDialingProgress;
	}
	re.dwfOptions2 = RASEO2_ReconnectIfDropped | RASEO2_DontNegotiateMultilink | RASEO2_UsePreSharedKey;
	re.dwFramingProtocol = RASFP_Ppp;
	re.dwHangUpExtraPercent = 10;
	re.dwHangUpExtraSampleSeconds = 120;
	re.dwIdleDisconnectSeconds = RASIDS_UseGlobalValue;
	re.dwRedialCount = 9999;
	re.dwRedialPause = 60;
	re.dwType = RASET_Vpn;
	re.dwVpnStrategy = VS_L2tpOnly;
	//re.dwIPv4InterfaceMetric = 1000;
	unsigned long  dns, dns2;
	dns = inet_addr(m_Config.dns1);
	dns2 = inet_addr(m_Config.dns2);
	memcpy(&re.ipaddrDns, &dns, sizeof(dns));
	memcpy(&re.ipaddrDnsAlt, &dns2, sizeof(dns2));

	wsprintf(re.szDeviceType, (L"vpn")); 
	wsprintf(re.szDeviceName, (L"WAN Miniport (L2TP)"));
	//USES_CONVERSION;
	wsprintf(re.szLocalPhoneNumber, (lpHost));
	DWORD dwResult = RasSetEntryProperties((L"conf.yvpn"), (lpName), &re, re.dwSize, NULL, 0);
	if (dwResult == 0)
	{
		rc.dwSize = sizeof(RASCREDENTIALSA);
		rc.dwMask = 16;
		wsprintf(rc.szUserName, (lpUser));
		wsprintf(rc.szPassword, (lpPass));
		dwResult = RasSetCredentials((L"conf.yvpn"), (lpName), &rc, 0);

		memset(&rc, 0, sizeof(RASCREDENTIALSA));
		rc.dwSize = sizeof(RASCREDENTIALSA);
		rc.dwMask = RASCM_PreSharedKey;
		wsprintf(rc.szPassword, lpL2tpKey);
		dwResult = RasSetCredentials((L"conf.yvpn"), (lpName), &rc, 0);

		if (dwResult == 0)
		{
			RASDIALPARAMS ra;
			ZeroMemory(&ra, sizeof(RASDIALPARAMS));
			wsprintf(ra.szEntryName, lpName);
			wsprintf(ra.szUserName, lpUser);
			wsprintf(ra.szPassword, lpPass);
			ra.dwSize = sizeof(RASDIALPARAMS);
			*h = NULL;
			dwResult = RasDial(NULL, L"conf.yvpn", &ra, 1, RasDialFunc, h);
		}
		if (dwResult != 0)
		{
			RasHangUp(*h);
		}
	}
	return dwResult;
}

void WINAPI CMainFrame::RasDialFunc(_In_  HRASCONN hrasconn,
	_In_  UINT unMsg,
	_In_  RASCONNSTATE rascs,
	_In_  DWORD dwError,
	_In_  DWORD dwExtendedError)
{
	wchar_t szRasString[MAX_PATH] = { 0 };
	if (dwError)
	{
		RasGetErrorString((UINT)dwError, szRasString, MAX_PATH);
		wchar_t szText[1024] = { 0 };
		wsprintf(szText, TEXT("连接失败：%d - %s ..."), dwError, szRasString);
		m_pThis->ShowText(szText);
		{
			std::map <int, PINFO>::iterator iter;
			for (iter = m_pThis->m_HostList.begin(); iter != m_pThis->m_HostList.end(); iter++)
			{
				PINFO pInfo = iter->second;
				if (pInfo->h == hrasconn)
				{
					RasHangUp(pInfo->h);
					pInfo->h = NULL;
					CString name;
					name.Format(_T("connbtn_%d"), pInfo->id);
					CButtonUI *pConn = (CButtonUI *)m_pThis->m_PaintManager.FindControl(name);
					pConn->SetText(_T("连接"));
					pConn->SetEnabled(true);
				}
			}
		}
		return;
	}
	wchar_t szText[1024] = { 0 };
	lstrcat(szText, TEXT("状态："));
	switch (rascs)
	{
	case RASCS_OpenPort:
		lstrcat(szText, L"正在打开端口...");
		break;
	case RASCS_PortOpened:
		lstrcat(szText, L"端口打开成功...");
		break;
	case RASCS_ConnectDevice:
		lstrcat(szText, L"正在连接到设备...");
		break;
	case RASCS_DeviceConnected:
		lstrcat(szText, L"设备连接成功...");
		break;
	case RASCS_AllDevicesConnected:
		lstrcat(szText, L"所以设备已连接成功...");
		break;
	case RASCS_Authenticate:
		lstrcat(szText, L"正在验证用户帐号...");
		break;
	case RASCS_AuthNotify:
		lstrcat(szText, L"正在验证信息...");
		break;
	case RASCS_AuthRetry:
		lstrcat(szText, L"验证失败！请检查用户帐号是否正确...");
		break;
	case RASCS_AuthCallback:
		lstrcat(szText, L"远程服务器请求认证回拨号码...");
		break;
	case RASCS_AuthChangePassword:
		lstrcat(szText, L"服务器要求修改用户密码...");
		break;
	case RASCS_AuthProject:
		lstrcat(szText, L"等待连接信息...");
		break;
	case RASCS_AuthLinkSpeed:
		lstrcat(szText, L"计算机连接速度...");
		break;
	case RASCS_AuthAck:
		lstrcat(szText, L"验证已接受...");
		break;
	case RASCS_ReAuthenticate:
		lstrcat(szText, L"回拨后重新验证...");
		break;
	case RASCS_Authenticated:
		lstrcat(szText, L"验证已完成...");
		break;

	case RASCS_PrepareForCallback:
		lstrcat(szText, L"已断开等待远程服务器回拨...\n");
		break;

	case RASCS_WaitForModemReset:
		lstrcat(szText, L"等待Modem重置...\n");
		break;

	case RASCS_WaitForCallback:
		lstrcat(szText, L"等待远程服务器回拨...\n");
		break;

	case RASCS_Projected:
		lstrcat(szText, L"映射完成...\n");
		break;

	case RASCS_StartAuthentication:
		lstrcat(szText, L"开始验证...\n");
		break;

	case RASCS_CallbackComplete:
		lstrcat(szText, L"回拨完成...\n");
		break;

	case RASCS_LogonNetwork:
		lstrcat(szText, L"登陆到网络...\n");
		break;

	case RASCS_SubEntryConnected:
		lstrcat(szText, L"子项目已连接...\n");
		break;

	case RASCS_SubEntryDisconnected:
		lstrcat(szText, L"子项目已断开...\n");
		break;
	case RASCS_Connected:
		//lstrcat(szText, L"连接已完成...\n");
		{
			std::map <int, PINFO>::iterator iter;
			for (iter = m_pThis->m_HostList.begin(); iter != m_pThis->m_HostList.end(); iter++)
			{
				PINFO pInfo = iter->second;
				
				if (pInfo->h == hrasconn)
				{
					lstrcat(szText, L"连接已完成...\n");

					CString name;
					name.Format(_T("connbtn_%d"), pInfo->id);
					CButtonUI *pConn = (CButtonUI *)m_pThis->m_PaintManager.FindControl(name);
					pConn->SetText(_T("断开"));
					pConn->SetEnabled(true);
				}
			}
		}
		break;

	case RASCS_Disconnected:
		lstrcat(szText, L"连接已断开...\n");
		{
			std::map <int, PINFO>::iterator iter;
			for (iter = m_pThis->m_HostList.begin(); iter != m_pThis->m_HostList.end(); iter++)
			{
				PINFO pInfo = iter->second;
				if (pInfo->h == hrasconn)
				{
					pInfo->h = NULL;
					CString name;
					name.Format(_T("connbtn_%d"), pInfo->id);
					CButtonUI *pConn = (CButtonUI *)m_pThis->m_PaintManager.FindControl(name);
					pConn->SetText(_T("连接"));
					pConn->SetEnabled(true);
				}
			}
		}
		break;
	default:
		lstrcat(szText, L"未知...\n");
		break;
	}
	m_pThis->ShowText(szText);
}


void CMainFrame::Connect(int id)
{
	PINFO pInfo = m_HostList[id];
	CString name;
	name.Format(_T("connbtn_%d"), id);
	CButtonUI *pConn = (CButtonUI *)m_PaintManager.FindControl(name);
	if (pInfo->h == NULL)
	{
		wchar_t szName[MAX_PATH] = { 0 };
		wsprintf(szName, TEXT("%s%d"), TEXT("YVPNConnect"), pInfo->id);
		pInfo->h = NULL;
		int ret = 0;

		if (pInfo->type == 1)
		{
			if (m_bXp)
			{
				char *lpszName = UnicodeToLocal(CP_ACP, szName);
				char *lpszUser = UnicodeToLocal(CP_ACP, pInfo->user.GetBuffer());
				char *lpszPass = UnicodeToLocal(CP_ACP, pInfo->pass.GetBuffer());
				char *lpszHost = UnicodeToLocal(CP_ACP, pInfo->host.GetBuffer());
				char *lpszL2tpkey = UnicodeToLocal(CP_ACP, pInfo->szL2tpKey.GetBuffer());
				ret = ConnectionL2tpVPNXP(lpszName, lpszUser, lpszPass, lpszHost, lpszL2tpkey, &pInfo->h, pInfo->crypt, m_Config.dwMode);
				delete[]lpszName;
				delete[]lpszUser;
				delete[]lpszPass;
				delete[]lpszHost;
				delete[]lpszL2tpkey;
			}
			else
			{
				ret = ConnectionL2tpVPN(szName, pInfo->user.GetBuffer(), pInfo->pass.GetBuffer(), pInfo->host.GetBuffer(), pInfo->szL2tpKey.GetBuffer(), &pInfo->h, pInfo->crypt, m_Config.dwMode);
			}
				
		}
		else
		{
			if (m_bXp)
			{
				char *lpszName = UnicodeToLocal(CP_ACP, szName);
				char *lpszUser = UnicodeToLocal(CP_ACP, pInfo->user.GetBuffer());
				char *lpszPass = UnicodeToLocal(CP_ACP, pInfo->pass.GetBuffer());
				char *lpszHost = UnicodeToLocal(CP_ACP, pInfo->host.GetBuffer());
				ret = ConnectionVPNXP(lpszName, lpszUser, lpszPass, lpszHost, &pInfo->h, pInfo->crypt, m_Config.dwMode);
				delete[]lpszName;
				delete[]lpszUser;
				delete[]lpszPass;
				delete[]lpszHost;
			}
			else
			{
				ret = ConnectionVPN(szName, pInfo->user.GetBuffer(), pInfo->pass.GetBuffer(), pInfo->host.GetBuffer(), &pInfo->h, pInfo->crypt, m_Config.dwMode);
			}
		}

		if (ret != 0)
		{
			wchar_t szText[MAX_PATH] = { 0 };
			wsprintf(szText, TEXT("连接失败，错误代码：%d ..."), ret);
			ShowText(szText);
			return;
		}

		pConn->SetText(_T("连接中."));
		pConn->SetEnabled(false);
		return;
	}
	if (MessageBox(GetHWND(),TEXT("是否真的断开？"), TEXT("提示"), MB_YESNO) == IDNO)
	{
		return;
	}
	int ret = RasHangUp(pInfo->h);
	if (ret == 0)
	{
		pInfo->h = NULL;
		pConn->SetText(_T("连接"));
		ShowText(TEXT("连接已经断开！"));
	}
	else
	{
		ShowText(TEXT("断开连接失败！"));
	}
	
}

void CMainFrame::ShowText(LPCTSTR szText)
{
	CLabelUI *pShowText = (CEditUI *)m_PaintManager.FindControl(_T("textshow"));
	pShowText->SetText(szText);
}

void CMainFrame::ShowTextSetting(LPCTSTR szText)
{
	CLabelUI *pShowText = (CEditUI *)m_PaintManager.FindControl(_T("textshow2"));
	pShowText->SetText(szText);
}

void CMainFrame::SaveConfig(PCONFIG pConfig)
{
	char szPath[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, szPath);
	lstrcatA(szPath, "\\yvpn.conf");
	CStringA mode;
	mode.Format("%d", pConfig->dwMode);
	WritePrivateProfileStringA("NETWORK", "dns1", pConfig->dns1, szPath);
	WritePrivateProfileStringA("NETWORK", "dns2", pConfig->dns2, szPath);
	WritePrivateProfileStringA("NETWORK", "mode", mode.GetBuffer(), szPath);
}

void CMainFrame::ReadConfig(PCONFIG pConfig)
{
	char szPath[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, szPath);
	lstrcatA(szPath, "\\yvpn.conf");
	GetPrivateProfileStringA(("NETWORK"), ("dns1"), ("8.8.8.8"), pConfig->dns1, MAX_PATH, szPath);
	GetPrivateProfileStringA(("NETWORK"), ("dns2"), ("8.8.4.4"), pConfig->dns2, MAX_PATH, szPath);
	pConfig->dwMode = GetPrivateProfileIntA(("NETWORK"), ("mode"), 1, szPath);
}