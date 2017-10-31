#pragma once

#include "node.h"

class CHostListItem
{
private:
	int m_Id;
	int m_Index;
	CString m_HostText;
	CString m_TypeText;
	CString m_PostionText;
	CString m_RemarkText;
	CString m_LogoText;
public:
	int GetId(){ return m_Id; }
	int GetIndex(){ return m_Index; }
	CString GetHostText(){return m_HostText;}
	CString GetTypeText(){ return m_TypeText; }
	CString GetPostionText(){ return m_PostionText; }
	CString GetRemarkText(){ return m_RemarkText;  }
	CString GetLogoText(){ return m_LogoText; }
	void SetId(int id){ m_Id = id; }
	void SetIndex(int id){ m_Index = id; }
	void SetHostText(CString szText){ m_HostText = szText; }
	void SetTypeText(CString szText){ m_TypeText = szText; }
	void SetPostionText(CString szText){ m_PostionText = szText; }
	void SetRemarkText(CString szText){ m_RemarkText = szText; }
	void SetLogoText(CString szText){ m_LogoText = szText; }
};

class CHostListUI :
	public CListUI
{
public:
	enum { SCROLL_TIMERID = 10 };
	CHostListUI(CPaintManagerUI& paint_manager);
	~CHostListUI();

	Node* AddNode(CHostListItem *item, Node* parent, int insertIndex = -1);
	void DoEvent(TEventUI& event);
	void RemoveAll();
	bool RemoveNode(Node* node);
	Node* GetRoot();
	CPaintManagerUI& paint_manager_;
private:
	
	CDialogBuilder m_dlgBuilder;
	LONG	delay_deltaY_;			//与滚动条滑动速度有关
	DWORD	delay_number_;
	DWORD	delay_left_;
	Node*	root_node_;				//父节点

	bool OnButtonEvent(void* param);
};

