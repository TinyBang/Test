#include"NFA.h"
NFA::NFA()
{
	firstNode=createNFA();
}
Node* NFA::createNFA()
{
	Node startNode;
	startNode.NodeNumber = 0;
	return &startNode;
}