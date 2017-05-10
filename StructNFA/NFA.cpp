#include"NFA.h"
NFA::NFA()
{
	firstNode=createNode();
}
Node* NFA::createNode() {
	Node newNode;
	AllNode.push_back(newNode);
	return &AllNode[AllNode.size()-1];
}
vector<Node*>NFA::NodeOr(int edge,  vector<Node*>HeadAndTail)
{
	Node *head = createNode();
	Node* a=createNode();
	Node *end = createNode();
	head->out1 = HeadAndTail[0];
	HeadAndTail[0] = head;
	HeadAndTail.push_back( end);
	head->out2 = a;
	end->State = 1;
	head->Edge = -1;
	a->out1 = end;
	a->Edge = edge;
	return HeadAndTail;
}
vector<Node*>NFA::NodeStar(vector<Node*>HeadAndTail)
{
	Node *head = createNode();
	Node *a = createNode(); 
	Node*b=createNode();
	head->out1 = HeadAndTail[0];
	HeadAndTail[0] = head;
	head->out2 = a;
	head->Edge = -1;
	a->out1 = b;
	a->Edge = -1;
	b->State = 1;
	int size = HeadAndTail.size();
	for (int i = size-1;i>0 ; i--)
	{
	HeadAndTail[i]->out1 = a;
	HeadAndTail[i]->Edge = -1;
	HeadAndTail.pop_back();
	}
	HeadAndTail.push_back(b);
	return HeadAndTail;
}
vector<Node*>NFA::NodePlus(vector<Node*>HeadAndTail)
{
	Node *head = createNode();
	Node *a = createNode();
	Node*b = createNode();
	head->out1 = HeadAndTail[0];
	HeadAndTail[0] = head;
	head->Edge = -1;
	a->out1 = b;
	a->Edge = -1;
	b->State = 1;
	int size = HeadAndTail.size();
	for (int i = size - 1; i>0; i--)
	{
		HeadAndTail[i]->out1 = a;
		HeadAndTail[i]->Edge = -1;
		HeadAndTail.pop_back();
	}
	HeadAndTail.push_back(b);
	return HeadAndTail;
}
void NFA::printNFA(vector<Node*>HeadAndTail)
{
	/*Path.push_back(HeadAndTail[0]);
	for (;;)
	{
		if (Path[Path.size() - 1]->out1 != NULL)
			Path.push_back(Path[Path.size() - 1]->out1);
		else
			break;
	}
	for (;;)
	{

	}*/

}
NFA NFA::structNFA(string EP)
{

	Node *a=createNode();
	firstNode->Edge = EP[0];
	firstNode->out1 = a;
	a->State = 1;
	HeadAndTail.push_back(firstNode);
	HeadAndTail.push_back(a);
	for (int i=1; i < EP.length(); i++)
	{
		if (EP[i] == '|')
		{
			HeadAndTail=NodeOr(EP[i - 1], HeadAndTail);
		}
		else if (EP[i]=='*')
		{
		HeadAndTail= NodeStar(HeadAndTail);
		}
		else
		{
			continue;
		}
	}

	cout << "Done!" << endl;
	return *this;
}
vector<Node*> NFA::getHeadAndTail()
{
	return HeadAndTail;
}
NFA NFA::linkNFA(NFA a, NFA b)
{

	for (int i = 0; i < (a.getHeadAndTail()).size(); i++)
	{
		(a.getHeadAndTail())[i]->out1 = (b.getHeadAndTail())[0];
		(a.getHeadAndTail())[i]->Edge = -1;
		(a.getHeadAndTail())[i]->State = 0;
	}
	return a;
}