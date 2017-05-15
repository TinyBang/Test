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
	vector<pair<int,int>> out;//指向的DFANode
	int state;//是否为可接受状态
	int NodeNumber;
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
};

#endif // !DFA_H
