#include"DFA.h"
vector<int> symbolTable;
#include<map>
int nodeNumber = 0;
void initSymbolTable()
{
	symbolTable.push_back('a');
	symbolTable.push_back('b');
	symbolTable.push_back('c');
}


bool isSameState(vector<int> v1, vector<DFANode> v)
{

	for (int i = 0; i < v.size(); i++)
	{
		vector<int> v2 = v[i].newNode;
		if (v1.size() != v2.size())
			continue;
		for (int i = 0; i < v1.size(); i++)
		{
			if (v1[i] != v2[i])
				break;
			if (i == v1.size() - 1)
				return true;
		}
	}
	return false;
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
DFANode DFA::turnNode(NFA nfa, int head)//根据NFA某个点构造epsion闭包
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
	initSymbolTable();
	int head = cnfa.HeadAndTail[0];
	vector<DFANode> dfaNode;
	DFANode nowNode = turnNode(cnfa, head);
	dfaNode.push_back(nowNode);
	for (int i = 0; i < dfaNode.size(); i++)
	{
		for (int j = 0; j < symbolTable.size(); j++)
		{
			nowNode = turnNode(cnfa, dfaNode[i].newNode, symbolTable[j]);
			if (nowNode.newNode.size() != 0&&!isSameState(nowNode.newNode,dfaNode))
			{
				dfaNode.push_back(nowNode);
				dfaNode[i].out.push_back(pair<int, int>(symbolTable[j], nodeNumber-1));
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

DFA DFA::minDFA(DFA cdfa)
{
	vector<DFANode> nodeList = cdfa.ALLNode;
	DFA miDfa;

	return miDfa;
}

