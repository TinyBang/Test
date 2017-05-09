#include"NFA.h"
int formalElement;
int nextElement;
int currentElement;
Node* currentNode;
void main()
{
	NFA newnfa;
	if (currentElement == '|')
	{
		Node* node1;
		Node* node2;
		currentNode->out1 = node1;
		currentNode->out2 = node2;
		currentNode->nodeType = 1;
		Node* node3;
		node3->nodeType = 1;
		Node* node4;
		node4->nodeType = 1;
		node1->out1 = node3;
		node1->Edge = formalElement;
		node2->out1 = node4;
		node2->Edge = nextElement;
	}
	if (currentElement = ='*')
	{

	}
	if(currentElement==)
}
Node* List SubNFA()
{

}