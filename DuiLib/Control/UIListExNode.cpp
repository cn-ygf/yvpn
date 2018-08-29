#include "stdafx.h"
#include "UIListExNode.h"

namespace DuiLib
{


	double CalculateDelay(double state)
	{
		return pow(state, 2);
	}

	Node::Node(void) : parent_(NULL)
	{
	}


	Node::~Node(void)
	{
		for (int i = 0; i < num_children(); ++i)
			delete children_[i];
	}

	void Node::set_parent(Node* parent)  //���ø��ڵ�
	{
		parent_ = parent;
	}

	Node::Node(NodeData t) : data_(t), parent_(NULL)		//���нڵ���Ϣ�Ĺ��캯��
	{

	}

	Node::Node(NodeData t, Node* parent) : data_(t), parent_(parent)	//���нڵ���Ϣ�͸��ڵ�Ĺ��캯��
	{

	}

	NodeData& Node::data()		//���� ����ڵ���Ϣ�Ľṹ��
	{
		return data_;
	}

	int Node::num_children() const			//��������
	{
		return static_cast<int>(children_.size());
	}

	Node* Node::child(int i)			//��i������
	{
		return children_[i];
	}

	Node* Node::parent()			//���ڵ�
	{
		return (parent_);
	}

	bool Node::has_children() const		//�Ƿ��к��ӽڵ�
	{
		return num_children() > 0;
	}

	bool Node::folder() const				//�Ƿ�folder
	{
		return data_.folder_;
	}

	void Node::add_child(Node* child)		//��Ӻ���
	{
		child->set_parent(this);
		children_.push_back(child);
	}

	void Node::remove_child(Node* child)			//ɾ������
	{
		Children::iterator iter = children_.begin();
		for (; iter < children_.end(); ++iter)
		{

			if (*iter == child)
			{
				children_.erase(iter);
				return;
			}
		}
	}

	Node* Node::get_last_child()			//��ȡ���һ������
	{
		if (has_children())
		{
			return child(num_children() - 1)->get_last_child();
		}
		return this;
	}

}