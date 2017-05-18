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
	int out1;
	int out2;
	int Edge;
	int State;
	int NodeNumber;
	string action="";
};
class NFA
{
public:
	NFA();
	NFA(string);
	NFA structNFA(string);
	vector<int>NodeOr( int edge, vector<int>HeadAndTail);
	vector<int>NodeStar(vector<int>HeadAndTail);
	vector<int> NodePlus(vector<int> HeadAndTail);
	void printNFA(vector<int> HeadAndTail);
	int createNode();
//	NFA linkNFA(NFA,NFA);
	vector<int> getHeadAndTail();
	vector<Node> AllNode;
//	NFA mergeNFA(NFA, NFA);
	vector<int> HeadAndTail;
	string action = "";
};
#endif // !NFA_H