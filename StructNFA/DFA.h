#pragma once
#ifndef DFA_H
#define DFA_H
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include"NFA.h"
#define nothing -1


struct DFANode
{
public:
	vector<int> newNode;//构成的NFANode
	vector<pair<int, int>> out;//指向的DFANode
	int state;//是否为可接受状态
	int NodeNumber;//节点编号
	string action;//节点语义
	DFANode()
	{
		state = 0;
		NodeNumber = -1;
		action = "";
	}

};

class DFA
{
public:
	DFA();
	DFA turnDFA(NFA);
	DFA minDFA(DFA);
	DFANode turnNode(NFA, int);
	DFANode turnNode(NFA, vector<int>, int);

	vector<DFANode> ALLNode;//所有DFANode的集合
	vector<int> HeadAndTail;//DFA的头和可接受状态
	vector<int> symbolTable;
	vector<int> createSymbolTable(vector<int> v1)
	{
		symbolTable = v1;
		return v1;
	}
};

#endif // !DFA_H
