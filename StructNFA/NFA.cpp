#include"NFA.h"
NFA::NFA(string action)
{
	this->action += action;
	createNode();
	HeadAndTail.push_back(0);
}
NFA::NFA()
{
	createNode();
	HeadAndTail.push_back(0);
};
int NFA::createNode() {
	Node newNode;
	newNode.Edge = -1;
	newNode.out1 = newNode.out2=-1;
	newNode.State = 0;
	newNode.action += action;
	AllNode.push_back(newNode);
	return AllNode.size()-1;
}
vector<int>NFA::NodeOr(int edge,  vector<int>HeadAndTail)
{
	int head = createNode();
	int  a=createNode();
	int end = createNode();
AllNode[head].out1 = HeadAndTail[0];
	HeadAndTail[0] =head;
	HeadAndTail.push_back( end);
	AllNode[head].out2 = a;
	AllNode[end].State = 1;
	AllNode[head].Edge = -1;
	AllNode[a].out1 =end;
	AllNode[a].Edge = edge;
	return HeadAndTail;
}
vector<int>NFA::NodeStar(vector<int>HeadAndTail)
{
	int head = createNode();
	int a = createNode();
	int b = createNode();
	AllNode[head].out1 = HeadAndTail[0];
	AllNode[head].out2 =b;
	AllNode[a].out1 = HeadAndTail[0];
	HeadAndTail[0] = head;
	AllNode[head].Edge = -1;
	AllNode[a].out2 = b;
	AllNode[a].Edge = -1;
	AllNode[b].State = 1;
	int size = HeadAndTail.size();
	for (int i = size - 1; i>0; i--)
	{
		AllNode[HeadAndTail[i]].out1 = a;
		AllNode[HeadAndTail[i]].Edge = -1;
		AllNode[HeadAndTail[i]].State = 0;
		HeadAndTail.pop_back();
	}
	HeadAndTail.push_back(b);
	return HeadAndTail;
}
vector<int>NFA::NodePlus(vector<int>HeadAndTail)
{
	int head = createNode();
	int a = createNode();
	int b = createNode();
	AllNode[head].out1 = HeadAndTail[0];
	AllNode[a].out1 = HeadAndTail[0];
	HeadAndTail[0] = head;
	AllNode[head].Edge = -1;
	AllNode[a].out2 = b;
	AllNode[a].Edge = -1;
	AllNode[b].State = 1;
	int size = HeadAndTail.size();
	for (int i = size - 1; i>0; i--)
	{
		AllNode[HeadAndTail[i]].out1 = a;
		AllNode[HeadAndTail[i]].Edge = -1;
		HeadAndTail.pop_back();
	}
	HeadAndTail.push_back(b);
	return HeadAndTail;
}
void NFA::printNFA(vector<int>HeadAndTail)
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
	int a=createNode();
	AllNode[0].Edge = EP[0];
	AllNode[0].out1 = a;
	AllNode[a].State = 1;
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
		else if (EP[i]=='+')
		{
			HeadAndTail = NodePlus(HeadAndTail);
		}
		else
		{
			continue;
		}
	}

	cout << "Done!" << endl;
	
	return *this;
}
vector<int> NFA::getHeadAndTail()
{
	return HeadAndTail;
}

