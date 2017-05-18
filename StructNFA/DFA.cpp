#include"DFA.h"
vector<int> symbolTable1;
#include<map>
int nodeNumber = 0;
void initSymbolTable1()
{
	symbolTable1.push_back('a');
	symbolTable1.push_back('b');
	symbolTable1.push_back('c');
}


int isSameState(vector<int> v1, vector<DFANode> v)
{

	for (int i = 0; i < v.size(); i++)
	{
		vector<int> v2 = v[i].newNode;
		if (v1.size() != v2.size())
			continue;
		for (int j = 0; j < v1.size(); j++)
		{
			if (v1[j] != v2[j])
				break;
			if (j== v1.size() - 1)
				return i;
		}
	}
	return -1;
}
bool isInState(int a, vector<int> v1)
{
	for (int i = 0; i < v1.size(); i++)
		if (a == v1[i])
			return true;

	return false;
}

DFA::DFA()
{

}
DFANode DFA::turnNode(NFA nfa, int head)//����NFAĳ���㹹��epsion�հ�
{
	DFANode dfaNode;
	vector<int> node;
	node.push_back(head);
	int change = 0;
	//while (true)
	//{
		for (int i = 0; i < node.size(); i++)
		{
			if (nfa.AllNode[node[i]].Edge == -1)
			{
				change = 1;
				if (!isInState(nfa.AllNode[node[i]].out1, node) && nfa.AllNode[node[i]].out1 != -1)
					node.push_back(nfa.AllNode[node[i]].out1);
				//cout << isInState(nfa.AllNode[node[i]].out2, node)<< nfa.AllNode[node[i]].out2;
				if (!isInState(nfa.AllNode[node[i]].out2, node) && nfa.AllNode[node[i]].out2 != -1)
					node.push_back(nfa.AllNode[node[i]].out2);
				if(nfa.AllNode[node[i]].out1==-1&& nfa.AllNode[node[i]].out2== -1)
					dfaNode.state = 1;
			}
		}

		//if (change == 0)
			//break;
		//change = 0;
	//}
	dfaNode.newNode = node;
	dfaNode.NodeNumber = nodeNumber;
	nodeNumber++;
	return dfaNode;
}
DFANode DFA::turnNode(NFA nfa, vector<int> head, int edge)
{
	DFANode dfaNode;
	vector<int> node;
	//map<int, vector<int>> edgeNode;
	for(int i=0;i<head.size();i++)
		if (nfa.AllNode[head[i]].Edge == edge)
			node.push_back(nfa.AllNode[head[i]].out1);
	for (int i = 0; i < node.size(); i++)
	{
		DFANode tmpNode = turnNode(nfa, node[i]);
		nodeNumber--;
		dfaNode.newNode.insert(dfaNode.newNode.end(), tmpNode.newNode.begin(), tmpNode.newNode.end());
		if (tmpNode.state == 1)
			dfaNode.state = 1;
	}
	//edgeNode[edge]= dfaNode.newNode();
	dfaNode.NodeNumber = nodeNumber;
	nodeNumber++;
	return dfaNode;
}

DFA DFA::turnDFA(NFA cnfa)
{
	initSymbolTable1();
	int head = cnfa.HeadAndTail[0];
	vector<DFANode> dfaNode;
	DFANode nowNode = turnNode(cnfa, head);
	dfaNode.push_back(nowNode);
	for (int i = 0; i < dfaNode.size(); i++)
	{
		for (int j = 0; j < symbolTable1.size(); j++)
		{
			nowNode = turnNode(cnfa, dfaNode[i].newNode, symbolTable1[j]);
			if (nowNode.newNode.size() != 0)
			{
				if (isSameState(nowNode.newNode, dfaNode)==-1)
				{
					dfaNode[i].out.push_back(pair<int, int>(symbolTable1[j], nodeNumber - 1));
					dfaNode.push_back(nowNode);
				}
				else
				{
					dfaNode[i].out.push_back(pair<int, int>(symbolTable1[j], isSameState(nowNode.newNode, dfaNode)));
					nodeNumber--;
				}
			}	
			else
			{
				nodeNumber--;
			}
		}
	}

	DFA dfa;
	dfa.ALLNode = dfaNode;
	return dfa;
}

/*bool changeNode(DFA dfa,DFANode dfaNode)
{
	for (int i = 0; i < dfaNode.newNode.size(); i++)
	{
		for (int j = 0; j < dfaNode.out.size(); j++)
		{
			for (int m = 0; m < symbolTable.size(); m++)
			{

			}
		}
		
	}
}*/

DFA DFA::minDFA(DFA cdfa)
{
	vector<DFANode> nodeList = cdfa.ALLNode;
	vector<vector<DFANode>> Node;
	DFANode term;
	DFANode unterm;
	for (int i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i].state == 1)
			term.newNode.push_back(i);
		else
			unterm.newNode.push_back(i);
	}
	while (true)
	{
		for (int i = 0; i < Node.size(); i++)
		{
			for (int j = 0; j < Node[i].size(); j++)
			{
				for (int m = 0; m < Node[i][j].out.size(); m++)
				{
					
				}
			}
		}
	}
	
	DFA miDfa;
	
	return miDfa;
}

