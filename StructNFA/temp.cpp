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
	vector<int> newNode;//���ɵ�NFANode
	vector<pair<int, int>> out;//ָ���DFANode
	int state;//�Ƿ�Ϊ�ɽ���״̬
	int NodeNumber;
};

class DFA
{
public:
	DFA() {};
	vector<DFANode> ALLNode;//����DFANode�ļ���
	vector<int> HeadAndTail;//DFA��ͷ�Ϳɽ���״̬
};
//��ʼ��DFA

int transState(int current,int edge,DFA dfa)
{
	int des;
	for (int i=0;i<dfa.ALLNode[current].out.size();i++)
	{
		if (dfa.ALLNode[current].out[i].first == edge)
			des = dfa.ALLNode[current].out[i].second;

	}
}

