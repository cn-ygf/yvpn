#pragma once

#include "node.h"

class CHostListItem
{
private:
	int m_Id;
	int m_Index;
	CDuiString m_HostText;
	CDuiString m_TypeText;
	CDuiString m_PostionText;
	CDuiString m_RemarkText;
	CDuiString m_LogoText;
public:
	int GetId(){ return m_Id; }
	int GetIndex(){ return m_Index; }
	CDuiString GetHostText(){return m_HostText;}
	CDuiString GetTypeText(){ return m_TypeText; }
	CDuiString GetPostionText(){ return m_PostionText; }
	CDuiString GetRemarkText(){ return m_RemarkText;  }
	CDuiString GetLogoText(){ return m_LogoText; }
	void SetId(int id){ m_Id = id; }
	void SetIndex(int id){ m_Index = id; }
	void SetHostText(CDuiString szText){ m_HostText = szText; }
	void SetTypeText(CDuiString szText){ m_TypeText = szText; }
	void SetPostionText(CDuiString szText){ m_PostionText = szText; }
	void SetRemarkText(CDuiString szText){ m_RemarkText = szText; }
	void SetLogoText(CDuiString szText){ m_LogoText = szText; }
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

