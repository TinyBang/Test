#pragma once
#include<iostream>
#include<vector>
#include<string>
#define nothing -1

using namespace std;

struct Node
{
	Node* out1=NULL;
	Node* out2=NULL;
	int Edge=-1;
	int State=0;
	//int NodeNumber;
};
class NFA
{
public:
	NFA();
	Node* createnewNFA();
	void structNFA(string);
	Node* firstNode;
	vector<Node*>NodeOr( int edge, vector<Node*>HeadAndTail);
	vector<Node*>NodeStar(vector<Node*>HeadAndTail);
	vector<Node*> NodePlus(vector<Node*> HeadAndTail);
	void printNFA(vector<Node*> HeadAndTail);
	Node* createNode();
private:
	vector<Node*> HeadAndTail;
	vector<Node> AllNode;
};