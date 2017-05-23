#pragma once
#ifndef NFA_H
#define NFA_H


#include<iostream>
#include<vector>
#include<string>
#define nothing -1

using namespace std;

struct Node
{
	int out1;//����1
	int out2;//����2
	int Edge;//�����ı�
	int State;//״̬
	string action="";//��Ӧ��״̬
};
class NFA
{
public:
	NFA();
	NFA(string);
	NFA structNFA(string);//����NFA
	vector<int>NodeOr( int edge, vector<int>HeadAndTail);//����|�����Ϊ
	vector<int>NodeStar(vector<int>HeadAndTail);//����*����Ϊ
	vector<int> NodePlus(vector<int> HeadAndTail);//����+����Ϊ
	int createNode();//����NFA��㣬���ҷ��ر��
//	NFA linkNFA(NFA,NFA);
	vector<int> getHeadAndTail();//����NFA������յ�
	vector<Node> AllNode;//��������NFA�Ľ��
//	NFA mergeNFA(NFA, NFA);
	vector<int> HeadAndTail;//��¼NFA����ʼ����յ㣬�±�Ϊ0����㣬��ߵ��е�
	string action = "";//����
};
#endif // !NFA_H