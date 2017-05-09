#pragma once
#include<iostream>

using namespace std;

struct Node
{
	Node* out1;
	Node* out2;
	int Edge;
	int nodeType=0;
	int State;
	int NodeNumber;
};
class NFA
{
public:
	NFA();
	Node* createNFA();
	Node* firstNode;
};