#pragma once

enum NodeType
{
	FRIENDS = 1000,
	PUBLIC_NO,
	GROUP_CHAT,
	LAST_CONTACT,
	OTHERS
};

struct NodeData
{
	CString host_text_;
	CString type_text_;
	CString postion_text_;
	CString remark_text_;
	bool folder_;
	CString text_; 
	NodeType  type_;
	CListContainerElementUI* list_elment_;
};

double CalculateDelay(double state);

class Node
{
public:
	Node(void);
	~Node(void);
public:
	explicit Node(NodeData t);
	Node(NodeData t, Node* parent);
	NodeData& data();
	int num_children() const;
	Node* child(int i);
	Node* parent();
	bool folder() const;
	bool has_children() const;
	void add_child(Node* child);
	void remove_child(Node* child);
	Node* get_last_child();
private:
	void set_parent(Node* parent);
private:
	typedef std::vector <Node*> Children;
	Children	children_;  //孩子节点集
	Node*		parent_;	//父节点
	NodeData    data_;	//自己节点的信息
};
