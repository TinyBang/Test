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
	int out1;//出边1
	int out2;//出边2
	int Edge;//经历的边
	int State;//状态
	string action="";//对应的状态
};
class NFA
{
public:
	NFA();
	NFA(string);
	NFA structNFA(string);//构建NFA
	vector<int>NodeOr( int edge, vector<int>HeadAndTail);//读到|后的行为
	vector<int>NodeStar(vector<int>HeadAndTail);//读到*的行为
	vector<int> NodePlus(vector<int> HeadAndTail);//读到+的行为
	int createNode();//创建NFA结点，并且返回编号
//	NFA linkNFA(NFA,NFA);
	vector<int> getHeadAndTail();//返回NFA的起点终点
	vector<Node> AllNode;//保存所有NFA的结点
//	NFA mergeNFA(NFA, NFA);
	vector<int> HeadAndTail;//记录NFA的起始点和终点，下标为0是起点，后边的中点
	string action = "";//动作
};
#endif // !NFA_H