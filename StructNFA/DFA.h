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
	vector<int> newNode;//���ɵ�NFANode
	vector<pair<int,int>> out;//ָ���DFANode
	int state;//�Ƿ�Ϊ�ɽ���״̬
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

	vector<DFANode> ALLNode;//����DFANode�ļ���
	vector<int> HeadAndTail;//DFA��ͷ�Ϳɽ���״̬
};

#endif // !DFA_H
