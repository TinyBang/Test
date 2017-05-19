#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include"NFA.h"
#define nothing -1
using namespace std;

vector<int> symbolTable;
int nodeNumber = 0;



struct DFANode
{
public:
	vector<int> newNode;//构成的NFANode
	vector<pair<int, int>> out;//指向的DFANode
	int state;//是否为可接受状态
	int NodeNumber;
};

class DFA
{
public:
	DFA() {};
	vector<DFANode> ALLNode;//所有DFANode的集合
	vector<int> HeadAndTail;//DFA的头和可接受状态
};
//初始化DFA

int transState(int current,int edge,DFA dfa)
{
	int des;
	for (int i=0;i<dfa.ALLNode[current].out.size();i++)
	{
		if (dfa.ALLNode[current].out[i].first == edge)
			des = dfa.ALLNode[current].out[i].second;

	}
}

