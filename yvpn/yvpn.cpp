// yvpn.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "yvpn.h"
#include "mainframe.h"



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
//#ifdef _DEBUG
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("res"));
/*#else
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CPaintManagerUI::SetResourceZip(_T("yvpn.res"));
#endif*/
	CMainFrame mainFrame;
	mainFrame.Create(NULL, _T("YVPN"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	mainFrame.CenterWindow();
	mainFrame.ShowModal();
	return 0;
}

