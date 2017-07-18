#include "stdafx.h"
#include "hostlistui.h"


CHostListUI::CHostListUI(CPaintManagerUI& paint_manager)
:paint_manager_(paint_manager),
delay_deltaY_(0),
delay_number_(0), 
delay_left_(0)
{
	root_node_ = new Node;
	root_node_->data().list_elment_ = NULL;
}


CHostListUI::~CHostListUI()
{
	RemoveAll();
	if (root_node_)
		delete root_node_;
	root_node_ = NULL;
}

static bool OnLogoButtonEvent(void* event)
{
	if (((TEventUI*)event)->Type == UIEVENT_BUTTONDOWN)
	{
		CControlUI* pButton = ((TEventUI*)event)->pSender;
		if (pButton != NULL)
		{
			CListContainerElementUI* pListElement = (CListContainerElementUI*)(pButton->GetTag());
			if (pListElement != NULL) pListElement->DoEvent(*(TEventUI*)event);
		}
	}
	return true;
}


Node* CHostListUI::AddNode(CHostListItem *item, Node* parent, int insertIndex)
{
	if (parent == NULL)
		parent = root_node_;

	CListContainerElementUI* pListElement = NULL;
	if (!m_dlgBuilder.GetMarkup()->IsValid())
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(L"host_list_item.xml", (UINT)0, NULL, &paint_manager_));
	else
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
	if (pListElement == NULL)
		return NULL; 

	Node* node = new Node;
	node->data().list_elment_ = pListElement;		//关联列表控件
	node->data().host_text_ = item->GetHostText();
	node->data().type_text_ = item->GetTypeText();
	node->data().postion_text_ = item->GetPostionText();
	node->data().remark_text_ = item->GetRemarkText();

	
	CLabelUI* pHost = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, _T("texthost")));
	if (pHost != NULL)
	{
		pHost->SetText(item->GetHostText());
	}
	CLabelUI* pType = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, _T("texttype")));
	if (pHost != NULL)
	{
		pType->SetText(item->GetTypeText());
	}
	CLabelUI* pPostion = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, _T("textpostion")));
	if (pPostion != NULL)
	{
		pPostion->SetText(item->GetPostionText());
	}
	CLabelUI* pRemark = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, _T("textremark")));
	if (pPostion != NULL)
	{
		pRemark->SetText(item->GetRemarkText());
	}

	CButtonUI* pLogo = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(pListElement, _T("logo")));
	if (pPostion != NULL)
	{
		pLogo->SetNormalImage(item->GetLogoText());
	}

	CButtonUI* pDel = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(pListElement, _T("delbtn")));
	if (pPostion != NULL)
	{
		CDuiString name;
		name.Format(_T("delbtn_%d"), item->GetId());
		pDel->SetName(name);
	}

	CButtonUI* pEdit = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(pListElement, _T("editbtn")));
	if (pPostion != NULL)
	{
		CDuiString name;
		name.Format(_T("editbtn_%d"), item->GetId());
		pEdit->SetName(name);
	}

	CButtonUI* pConn = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(pListElement, _T("connbtn")));
	if (pConn != NULL)
	{
		CDuiString name;
		name.Format(_T("connbtn_%d"), item->GetId());
		pConn->SetName(name);
	}

	pDel->OnEvent += DuiLib::MakeDelegate(this, &CHostListUI::OnButtonEvent);
	pEdit->OnEvent += DuiLib::MakeDelegate(this, &CHostListUI::OnButtonEvent);
	pConn->OnEvent += DuiLib::MakeDelegate(this, &CHostListUI::OnButtonEvent);

	CVerticalLayoutUI* pOption = static_cast<CVerticalLayoutUI*>(paint_manager_.FindSubControlByName(pListElement, _T("option_layout")));
	if (pOption != NULL)
	{
		CDuiString name;
		name.Format(_T("option_layout_%d"), item->GetIndex());
		pOption->SetName(name);
	}

	pListElement->SetTag((UINT_PTR)node);
	pListElement->SetFixedHeight(60);
		
	int index = 0;
	if (parent->has_children())
	{
		Node* prev = parent->get_last_child();
		index = prev->data().list_elment_->GetIndex() + 1;
	}
	else
	{
		if (parent == root_node_)
			index = 0;
		else
			index = parent->data().list_elment_->GetIndex() + 1;
	}
	bool ret = CListUI::AddAt(pListElement, insertIndex == -1 ? index : insertIndex);
	if (ret == false)
	{
		delete pListElement;
		delete node;
		node = NULL;
	}
	parent->add_child(node);
	return node;
}

void CHostListUI::DoEvent(TEventUI& event)
{
	// 	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
	// 	{
	// 		if (m_pParent != NULL)
	// 			m_pParent->DoEvent(event);
	// 		else
	// 			CVerticalLayoutUI::DoEvent(event);
	// 		return;
	// 	}
	if (event.Type == UIEVENT_MOUSEENTER)
	{
		int a = 1;
	}
	if (event.Type == UIEVENT_TIMER && event.wParam == SCROLL_TIMERID)
	{
		if (delay_left_ > 0)
		{
			--delay_left_;
			SIZE sz = GetScrollPos();
			LONG lDeltaY = (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
			if ((lDeltaY > 0 && sz.cy != 0) || (lDeltaY < 0 && sz.cy != GetScrollRange().cy))
			{
				sz.cy -= lDeltaY;
				SetScrollPos(sz);
				return;
			}
		}
		delay_deltaY_ = 0;
		delay_number_ = 0;
		delay_left_ = 0;
		m_pManager->KillTimer(this, SCROLL_TIMERID);
		return;
	}
	if (event.Type == UIEVENT_SCROLLWHEEL)
	{
		LONG lDeltaY = 0;
		if (delay_number_ > 0)
			lDeltaY = (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
		switch (LOWORD(event.wParam))
		{
		case SB_LINEUP:
			if (delay_deltaY_ >= 0)
				delay_deltaY_ = lDeltaY + 7;
			else
				delay_deltaY_ = lDeltaY + 11;
			break;
		case SB_LINEDOWN:
			if (delay_deltaY_ <= 0)
				delay_deltaY_ = lDeltaY - 7;
			else
				delay_deltaY_ = lDeltaY - 11;
			break;
		}
		if
			(delay_deltaY_ > 100) delay_deltaY_ = 100;
		else if
			(delay_deltaY_ < -100) delay_deltaY_ = -100;

		delay_number_ = (DWORD)sqrt((double)abs(delay_deltaY_)) * 5;
		delay_left_ = delay_number_;
		m_pManager->SetTimer(this, SCROLL_TIMERID, 20U);
		return;
	}

	CListUI::DoEvent(event);
}

void CHostListUI::RemoveAll()
{
	CListUI::RemoveAll();
	for (int i = 0; i<root_node_->num_children();)
	{
		Node* child = root_node_->child(i);
		RemoveNode(child);
	}
	delete root_node_;

	root_node_ = new Node;
	root_node_->data().list_elment_ = NULL;
}

bool CHostListUI::RemoveNode(Node* node)
{
	if (!node || node == root_node_)
		return false;
	for (int i = 0; i<node->num_children(); ++i)
	{
		Node* child = node->child(i);
		RemoveNode(child);
	}

	CListUI::Remove(node->data().list_elment_);

	node->parent()->remove_child(node);
	delete node;
	node = NULL;
	return true;
}

Node* CHostListUI::GetRoot()
{
	return root_node_;
}

bool CHostListUI::OnButtonEvent(void* param)
{
	TEventUI* pEvent = (TEventUI*)param;
	if (pEvent->Type == UIEVENT_SETCURSOR) {
		return false;
	}
	return true;
}