#include"NFA.h"

/*void main(string Suffix)
{
	
	int i = 0;
	NFA newnfa;

	
}*/
vector<Node*>NodeOr(char currentoperator,int edge,Node* formalNode,vector<Node*>HeadAndTail)
{
		Node head, a ;
		Node end;
		head.out1 = HeadAndTail[0];
		HeadAndTail[0] = &head;
		HeadAndTail[HeadAndTail.size()] = &end;
		head.out2 = &a;
		head.Edge = -1;
		a.out1 = &end;
		a.Edge = edge;
		return HeadAndTail;
}