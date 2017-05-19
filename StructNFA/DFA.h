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
	vector<pair<int, int>> out;//ָ���DFANode
	int state;//�Ƿ�Ϊ�ɽ���״̬
	int NodeNumber;//�ڵ���
	string action;//�ڵ�����
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

	vector<DFANode> ALLNode;//����DFANode�ļ���
	vector<int> HeadAndTail;//DFA��ͷ�Ϳɽ���״̬
	vector<int> symbolTable;
	vector<int> createSymbolTable(vector<int> v1)
	{
		symbolTable = v1;
		return v1;
	}
};

#endif // !DFA_H
